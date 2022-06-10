/**
 * @file Controller.h
 * @author Kevin Steiger (steigkev@students.zhaw.ch)
 * @brief Controller header file
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef BA_CONTROLLER_H
#define BA_CONTROLLER_H

#include <unistd.h>
#include <JetsonGPIO.h>

/**
 * @brief Class for receiving control messages and convert them into PWM signals
 * 
 */
class Controller {
public:
        Controller();
        static void setup();
        void process(char cmd);
        void g_set_speed(int value);
        void g_set_steering(int value);

private:
    static const int PIN_STEER;                     
    static const int PIN_SPEED;                     
    static const int PWM_FREQ;                      

    static const int STEER_MIDDLE;                  
    static const int STEER_MOST_RIGHT;
    static const int STEER_MOST_LEFT;
    static const int SPEED_MIDDLE;
    static const int SPEED_MOST_FORWARD;
    static const int SPEED_MOST_BACKWARD;

    int steer_state;
    int speed_state;
    GPIO::PWM steer_pwm;
    GPIO::PWM speed_pwm;
};

#endif //BA_CONTROLLER_H
