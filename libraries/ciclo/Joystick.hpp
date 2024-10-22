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
#include "soc/sens_reg.h" // needed for manipulating ADC2 control register

class Joystick {
    private:
        int pin;
		uint32_t adc_register;
		uint32_t wifi_register;

    public:
        Joystick(int pin, uint32_t adc_register, uint32_t wifi_register);

        int get_power();//maybe unnecessary

        bool right();

        bool left();

		bool middle();
};

#endif  // __JOYSTICK_HPP__
