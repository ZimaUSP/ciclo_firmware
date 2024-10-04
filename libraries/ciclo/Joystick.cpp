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


#include "Joystick.hpp"
/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/
Joystick::Joystick(int pin) {
  this->pin=pin;
}

int Joystick::get_power() {
    return analogRead(this->pin);
}

bool Joystick::left() {
    if (analogRead(this->pin) >= 0 && analogRead(this->pin) <= 100)
        return true;
    return false;
}

bool Joystick::right() {
    if (analogRead(this->pin) >= 3995 && analogRead(this->pin) <= 4095)
        return true;
    return false;
}