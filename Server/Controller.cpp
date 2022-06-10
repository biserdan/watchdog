/**
 * @file Controller.cpp
 * @author Kevin Steiger (steigkev@students.zhaw.ch)
 * @brief Controller source file
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Controller.h"

const int Controller::PIN_STEER = 33;                   //!< Pin number for steering
const int Controller::PIN_SPEED = 32;                   //!< Pin number for speed
const int Controller::PWM_FREQ = 400;                   //!< PWM frequency for steering and speed

const int Controller::STEER_MIDDLE = 56;                //!< Duty cycle for neutral steering positon 
const int Controller::STEER_MOST_RIGHT = 30;            //!< Duty cycle for max right steering positon 
const int Controller::STEER_MOST_LEFT = 82;             //!< Duty cycle for max left steering positon 
const int Controller::SPEED_MIDDLE = 60;                //!< Duty cycle for neutral speed positon 
const int Controller::SPEED_MOST_FORWARD = 75;          //!< Duty cycle for max forward speed positon 
const int Controller::SPEED_MOST_BACKWARD = 45;         //!< Duty cycle for max backwar speed positon 

const int MAX_GAMEPAD_RANGE  = 32768;                   //!< Max value for deflection / pressure level of joystick / button

/**
 * @brief Construct a new Controller:: Controller object
 * 
 */
Controller::Controller() : steer_state(STEER_MIDDLE), speed_state(SPEED_MIDDLE), steer_pwm(GPIO::PWM(PIN_STEER, PWM_FREQ)),
                           speed_pwm(GPIO::PWM(PIN_SPEED, PWM_FREQ)){
    this->steer_pwm.start(STEER_MIDDLE);
    this->speed_pwm.start(SPEED_MIDDLE);
}

/**
 * @brief Cleans old PWM settings and sets PWM pins
 * 
 */
void Controller::setup() {
    GPIO::cleanup();
    GPIO::setmode(GPIO::BOARD);
    GPIO::setup(PIN_STEER, GPIO::OUT);
    GPIO::setup(PIN_SPEED, GPIO::OUT);
}

/**
 * @brief Processes incoming messages and converts them into PWM signals
 * 
 * @param cmd Contains the control message
 */
void Controller::process(char cmd) {
    if (cmd == 'd') {//right
        if (this->steer_state - 1 > STEER_MOST_RIGHT) {
            this->steer_state -= 1;
            this->steer_pwm.ChangeDutyCycle(this->steer_state);
        }
    } else if (cmd == 'a') {//left
        if (this->steer_state + 1 < STEER_MOST_LEFT) {
            this->steer_state += 1;
            this->steer_pwm.ChangeDutyCycle(this->steer_state);
        }
    } else if (cmd == 'w') { //forward
        if (this->speed_state + 1 < SPEED_MOST_FORWARD) {
            this->speed_state += 1;
            this->speed_pwm.ChangeDutyCycle(this->speed_state);
        }
    } else if (cmd == 's') { //backward
        if (this->speed_state - 1 > SPEED_MOST_BACKWARD) {
            this->speed_state -= 1;
            this->speed_pwm.ChangeDutyCycle(this->speed_state);
        }
    } else if (cmd == 'x') { //stop
        this->steer_pwm.ChangeDutyCycle(STEER_MIDDLE);
        this->speed_pwm.ChangeDutyCycle(SPEED_MIDDLE);
        this->steer_state = STEER_MIDDLE;
        this->speed_state = SPEED_MIDDLE;
    } else if (cmd == 'z') { //exit
        this->steer_pwm.ChangeDutyCycle(STEER_MIDDLE);
        this->speed_pwm.ChangeDutyCycle(SPEED_MIDDLE);

        sleep(1);
        this->steer_pwm.stop();
        this->speed_pwm.stop();
        GPIO::cleanup();
        printf("GPIO cleanup\n");
    } else {
        printf("command not supported\n");
    }
    printf("steer position: %d\n", this->steer_state);
    printf("speed position: %d\n", this->speed_state);
}

/**
 * @brief Sets the RC car speed for game controller messages
 * 
 * @param value Contains the deflection / pressure level for the joystick / button
 */
void Controller::g_set_speed(int value) {
    int duty_cycle = SPEED_MIDDLE;

    if (value) {
        duty_cycle +=  (SPEED_MOST_FORWARD-SPEED_MIDDLE)*((float)value/MAX_GAMEPAD_RANGE);
    } else {
        duty_cycle +=  (SPEED_MOST_BACKWARD-SPEED_MIDDLE)*((float)value/MAX_GAMEPAD_RANGE);
    }

    printf("speed: %d\n", duty_cycle);
    return;

    this->speed_pwm.ChangeDutyCycle(duty_cycle);
    this->speed_state = duty_cycle;

    printf("steer position: %d\n", this->steer_state);
    printf("speed position: %d\n", this->speed_state);
}

/**
 * @brief Sets the RC car steering for game controller messages
 * 
 * @param value Contains the deflection / pressure level for the joystick / button
 */
void Controller::g_set_steering(int value) {
    int duty_cycle = STEER_MIDDLE;

    if (value) {
        duty_cycle +=  (STEER_MOST_RIGHT-STEER_MIDDLE)*((float)value/MAX_GAMEPAD_RANGE);
    } else {
        duty_cycle +=  (STEER_MOST_LEFT-STEER_MIDDLE)*((float)value/MAX_GAMEPAD_RANGE);
    }

    printf("speed: %d\n", duty_cycle);
    return;

    this->steer_pwm.ChangeDutyCycle(duty_cycle);
    this->steer_state = duty_cycle;

    printf("steer position: %d\n", this->steer_state);
    printf("speed position: %d\n", this->speed_state);
}