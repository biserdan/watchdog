/**
 * @file Gamepad.h
 * @author Kevin Steiger (steigkev@students.zhaw.ch)
 * @brief Gamepad header file
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef BA_GAMEPAD_H
#define BA_GAMEPAD_H

#include <SDL2/SDL.h>
#include <string>
#include "UDP_Client.h"

/**
 * @brief Class for using a ps3 controller
 * 
 */
class Gamepad {
public:
    Gamepad(UDP_Client &client);
    ~Gamepad();
    void fetch();

private:
    SDL_Joystick * gGameController;
    SDL_Event event;
    UDP_Client client;
};


#endif //BA_GAMEPAD_H
