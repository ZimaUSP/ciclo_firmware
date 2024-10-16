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
        uint8_t offset;
        float sensitivity;
    public:
        /**
         * @brief Constructor for the current sensor class
         */
        current_sensor(uint8_t pin, uint8_t offset, uint8_t type);

        /**
         * @brief Initializes the current sensor
         */
        void init();

        /**
         * @brief Reads and returns the current
         */
        float get_current();

        /**
         * @brief Calculates and returns the torque based on the current
         */
        float get_torque(float acs);
};

#endif  // __current_sensor_HPP__
