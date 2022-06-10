/**
 * @file client.cpp
 * @author Kevin Steiger (steigkev@students.zhaw.ch)
 * @brief Client application main source file
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <iostream>
#include <thread>
#include <atomic>

#include "Gamepad.h"
#include "UDP_Client.h"

#define SERVER_ADDR "ba22openpilot.zhaw.ch"     //!< Server address for Nvidia Jetson Nano
#define SERVER_PORT 1234                        //!< Server port for watchdog messages
#define SERVER_PORT_CMD 2468                    //!< Server port for keyboard messages
#define SERVER_PORT_GAME 3333                   //!< Server port for game controller messages
#define WD_INTERVAL 20000                       //!< Sending time interval for watchdog

using namespace std;
atomic<bool> stop_token(false); // TODO Needs to be replaced by a conditional variable

/**
 * @brief Sends a single char to the server watchdog
 * 
 * @param client Contains the client socket for sending packets
 */
void kick_watchdog(UDP_Client &client) {
    int rc;
    char txbuf[10];

    sprintf(txbuf, "%s", "o");
    client.send(txbuf, strlen(txbuf)+1);

    while (!stop_token) {
        usleep(WD_INTERVAL);
        rc = client.send(txbuf, strlen(txbuf) + 1);
        if (rc < 0) {
            printf("Unable to send data\n");
            printf("recv error %d\n", errno);
            break;
        }
    }
}

/**
 * @brief Sends control signals for the keyboard as char to the server. 
 * 
 * @param client Contains the client socket for sending packets
 */
void send_commands(UDP_Client &client) {
    char cmd;
    int rc;

    cout << "w: forward\ta: left\n";
    cout << "s: backward\td: right\n";
    cout << "x: neutral\tz: exit\n";

    while (true) {
        cout << "Type in command: ";
        cin >> cmd;

        if (cmd != 'w' && cmd != 's' && cmd != 'x' && cmd != 'a' && cmd != 'd' && cmd != 'z') {
            cout << "Command not supported\n";
        } else {
            rc = client.send(&cmd, sizeof(char));
            if (rc < 0) {
                printf("Unable to send data\n");
                break;
            }
        }

        if (cmd == 'z') {
            cout << "Program closed\n";
            stop_token = true;
            break;
        }

    }
}

int main(int argc, char **argv) {

    UDP_Client c_watchdog(SERVER_PORT, SERVER_ADDR);
    UDP_Client c_keyboard(SERVER_PORT_CMD, SERVER_ADDR);
    UDP_Client c_gamepad(SERVER_PORT_GAME, SERVER_ADDR);

    Gamepad gamepad(c_gamepad);

    thread t_watchdog(kick_watchdog, std::ref(c_watchdog));
    thread t_controller(send_commands, std::ref(c_keyboard));
    thread t_gamepad(&Gamepad::fetch, std::ref(gamepad));

    t_gamepad.join();
    t_watchdog.join();
    t_controller.join();

    return EXIT_SUCCESS;
}