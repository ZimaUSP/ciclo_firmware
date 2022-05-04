/**
 * @file  current_sensor.hpp
 *
 * @brief Encoder class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 05/2022
 * 
 * @copyright MIT License
 */

#ifndef __current_sensor_HPP__
#define __current_sensor_HPP__

#include "Arduino.h"

class current_sensor {
    private:
        uint8_t pin;
        uint8_t type;
        float sensitivity;
    public:
        /**
         * @brief Default constructor of a Timer base class
         * 
         */
        current_sensor();
    
        /**
         * @brief return left to complet period (minutes part)
         */
        float get_current();

};

#endif  // __current_sensor_HPP__