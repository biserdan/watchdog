/**
 * @file server.cpp
 * @author Kevin Steiger (steigkev@students.zhaw.ch)
 * @brief Server application main source file
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include <iostream>
#include <thread>
#include <atomic>

#include <JetsonGPIO.h>
#include "Controller.h"
#include "UDP_Server.h"


#define WATCHDOG_PORT 1234              //!< Server port port for watchdog messages
#define CONTROL_PORT 2468               //!< Server port port for keyboard messages
#define GAMEPAD_PORT 3333               //!< Server port port for game controller messages

#define BUF 255                         //!< Buffer size for incomming messages
#define TIMEOUT_USEC 100000             //!< Timeout for incomming messages in microseconds

std::atomic<bool> start_token(false);

/**
 * @brief Receives watchdog messages and terminates the program if messages are missing
 * 
 * @param server Contains the server socket for receiving packets
 * @param controller Contains the controller instance for setting PWM signals
 */
void w_receive(UDP_Server &server, Controller &controller) {
    char puffer[BUF];
    int n;

    int exit_buffer = 0;

    printf("Waiting for data (UDP)... \n");

    do {
        // initialise puffer
        memset(puffer, 0, BUF);

        n = server.receive(puffer, BUF);
    } while (n < 0);

    printf("Connection established with: %s\n", inet_ntoa(server.cliAddr.sin_addr));

    start_token = true;

    while (true) {
        // initialise puffer
        memset(puffer, 0, BUF);

        n = server.receive(puffer, BUF);

        if (n < 0) {
            exit_buffer++;
            if (exit_buffer == 3) {
                controller.process('z');
                printf("Connection lost\n");
                printf("Socket Receive Error: %d\n", errno);
                // break;
                exit(EXIT_FAILURE);
            }
        } else {
            exit_buffer = 0;
        }
    }
}

/**
 * @brief Receives keyboard messages sends them on to the controller instance
 * 
 * @param server Contains the server socket for receiving packets
 * @param controller Contains the controller instance for setting PWM signals
 */
void c_receive(UDP_Server &server, Controller &controller) {
    char puffer[BUF];
    int n;

    while (true) {
        // initialise puffer
        memset(puffer, 0, BUF);

        n = server.receive(puffer, BUF);

        if (n < 0) {
            printf("Missing data ...\n");
            printf("Socket Receive Error %d\n", errno);
        }

        controller.process(puffer[0]);
        if(puffer[0] == 'z')
            break;
    }
}

/**
 * @brief Receives game controller messages sends them on to the controller instance
 * 
 * @param server Contains the server socket for receiving packets
 * @param controller Contains the controller instance for setting PWM signals
 */
void g_receive(UDP_Server &server, Controller &controller) {
    char puffer[BUF];
    int n, value;
    char axis;

    while (true) {
        // initialise puffer
        memset(puffer, 0, BUF);

        n = server.receive(puffer, BUF);

        if (n < 0) {
            printf("Missing data ...\n");
            printf("Socket Receive Error %d\n", errno);
        }

        sscanf(puffer, "%c %d", &axis, &value);

        if (axis == 'y')
            controller.g_set_speed(value);
        else if (axis == 'x')
            controller.g_set_steering(value);
    }
}

int main(int argc, char** argv) {

    Controller::setup();
    Controller controller;

    UDP_Server w_server(WATCHDOG_PORT, TIMEOUT_USEC);
    UDP_Server c_server(CONTROL_PORT);
    UDP_Server g_server(GAMEPAD_PORT);

    std::thread w_thread(w_receive, std::ref(w_server), std::ref(controller));

    while(start_token == false);

    std::thread c_thread(c_receive, std::ref(c_server), std::ref(controller));
    std::thread g_thread(g_receive, std::ref(g_server), std::ref(controller));

    w_thread.join();
    c_thread.join();
    g_thread.join();

    printf("Closing Server\n");
    
    return EXIT_SUCCESS;
}