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
        bool reverse_logic;
        int pin;

    public:
        /**
         * @brief Default constructor of a encoder base class
         * 
         * @param pin digital pin that comands relay 
         */
        rele(int pin,bool reverse_logic);

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