/**
 * @file  current_sensor.cpp
 *
 * @brief Encoder class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 05/2022
 * 
 * @copyright MIT License
 */


#include "current_sensor.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

/**
 * @brief Constructor for the current sensor class
 */
current_sensor::current_sensor(uint8_t pin, uint8_t offset, uint8_t type) {
    this->pin = pin;
    this->type = type;
    this->offset = offset;
    if (type == 5) {
        this->sensitivity = 0.185;
    } else if (type == 20) {
        this->sensitivity = 0.095;
    } else if (type == 30) {
        this->sensitivity = 0.072;
    }
}

/**
 * @brief Initializes the current sensor
 */
void current_sensor::init() {
    pinMode(this->pin, INPUT);  // Configura o pino como entrada
}

/**
 * @brief Reads and returns the current
 */
float current_sensor::get_current() {
    int sum = 0; 
    for (int i = 0; i < 100; i++) {
        sum += analogRead(this->pin);
        delay(10);
    }
    Serial.print("Média:");
    Serial.print(sum / 100);
    return (float)((sum / 100) - 1850) * (1.39) / (122) - 0.20000;
}

/**
 * @brief Calculates and returns the torque based on the current
 */
float current_sensor::get_torque(float acs) {
    return (float)(0.4 * acs - 0.2);
}
