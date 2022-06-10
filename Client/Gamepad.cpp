/**
 * @file Gamepad.cpp
 * @author Kevin Steiger (steigkev@students.zhaw.ch)
 * @brief Gamepad source file
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Gamepad.h"
#include <atomic>

extern std::atomic<bool> stop_token; // TODO Needs to be replaced by a conditional variable

/**
 * @brief Construct a new Gamepad:: Gamepad object
 * 
 * @param client Contains the client UDP socket
 */
Gamepad::Gamepad(UDP_Client &client) : client(client){
    gGameController = NULL;

    // Init SDL
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        printf("Info: SDL init successful\n");
    }

    // Check for joysticks and load it
    if (SDL_NumJoysticks() < 1) {
        printf("Warning: No joysticks connected!\n");
    } else {
        gGameController = SDL_JoystickOpen(0);
        if (gGameController == NULL) {
            printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
        } else {
            printf("Info: Open game controller: success\n");
        }
    }
}

/**
 * @brief Destroy the Gamepad:: Gamepad object
 * 
 */
Gamepad::~Gamepad() {
    SDL_JoystickClose(gGameController);
    gGameController = NULL;
    SDL_Quit();
}

/**
 * @brief Polls button events and sends corresponding control signal
 * 
 * Motion on joystick buttons are polled with SDL. Actions on left joystick are
 * meant for movements in longitudinal direction, actions on right joystick for 
 * movements in lateral direction. Both events are send to the server.
 * 
 */
void Gamepad::fetch() {
    bool quit = false;
    int rc = 0;

    while (!stop_token) {
        while (SDL_PollEvent(&event) != 0) {
            // User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_JOYAXISMOTION) {
                if (event.jaxis.which == 0) {
                    // Motion on left joystick Y axis
                    if (event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                        char txbuf[10];
                        sprintf(txbuf, "y %d", event.jaxis.value);
                        rc = client.send(txbuf, strlen(txbuf)+1);
                        if (rc < 0) {
                            printf("Unable to send data\n");
                            printf("socket error %d\n", errno);
                        }
                        printf("%s\n", txbuf);
                    } 
                    // Motion on right joystick X axis
                    else if (event.jaxis.axis == 3) {
                        char txbuf[10];
                        sprintf(txbuf, "x %d", event.jaxis.value);
                        rc = client.send(txbuf, strlen(txbuf) + 1);
                        if (rc < 0) {
                            printf("Unable to send data\n");
                            printf("socket error %d\n", errno);
                        }
                        printf("%s\n", txbuf);
                    }
                }
            }
            //fflush(stdout);
        }
    }
}
