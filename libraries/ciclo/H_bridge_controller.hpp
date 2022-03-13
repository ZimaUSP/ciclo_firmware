/**
 * @file  H_bridge_controller.cpp
 *
 * @brief H_bridge_controller Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */

#ifndef __H_bridge_controller_HPP__
#define __H_bridge_controller_HPP__

#include "Arduino.h"

class H_bridge_controller {
    private:

        int R_pin;
        int L_pin;


    public:

        /**
         * @brief Default constructor of a H_bridge_controller base class
         * 
         * @param R_pin R PMW pin 
         * @param L_pin L PWM pin 
         *
         */
        H_bridge_controller(int R_pin, int L_pin);

        /**
         * @brief Init PWM pins
         */
        void init();
    
        /**
         * @brief Set PWM right duty cicle
         */
        void SetPWM_R(int PWM);

        /**
         * @brief Set PWM left duty cicle
         */
        void SetPWM_L(int PWM);

        /**
         * @brief Set right PWM
         */
        void Set_R(int PWM);

        /**
         * @brief Set left PWM
         */
        void Set_L(int PWM);

};

#endif  // __H_bridge_controller_HPP__