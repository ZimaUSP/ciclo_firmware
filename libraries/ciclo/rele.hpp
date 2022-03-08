/**
 * @file  rele.cpp
 *
 * @brief rele Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 02/2022
 * 
 * @copyright MIT License
 */

#ifndef __rele_HPP__
#define __rele_HPP__

#include "Arduino.h"

class rele {
    private:

        int pin;

    public:
        /**
         * @brief Default constructor of a encoder base class
         * 
         * @param pin digital pin that comands relay 
         */
        rele(int pin);

        /**
         * @brief initialize pin
         */
        void init();
    
        /**
         * @brief comun and normaly close are now short circuited
         */
        void turn_on();

        /**
         * @brief comun and normaly open are now short circuited
         */
        void turn_off();



};

#endif  // __rele_HPP__