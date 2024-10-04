/**
 * @file  Joystick.cpp
 *
 * @brief Joystick Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */

#ifndef __JOYSTICK_HPP__
#define __JOYSTICK_HPP__

#include "Arduino.h"

class Joystick {
    private:
        int pin;

    public:
        Joystick(int pin);

        int get_power();//maybe unnecessary

        bool right();

        bool left();

};

#endif  // __JOYSTICK_HPP__