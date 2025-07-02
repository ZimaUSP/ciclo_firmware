/**
 * @file  Joystick.cpp
 *
 * @brief Joystick class
 *
 * @author Guilherme Tokio Kawahara <guitokio@usp.br>
 *
 * @date 10/2024
 * 
 * @copyright MIT License
 */


#include "Joystick.hpp"
/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/
Joystick::Joystick(int pin, uint32_t adc_register, uint32_t wifi_register) {
  this->pin = pin;
  this->adc_register = adc_register;
  this->wifi_register = wifi_register;
}

int Joystick::get_power() {
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, this->adc_register); // Wifi with ADC2 on ESP32 workaround.
  SET_PERI_REG_MASK(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_DATA_INV);// Wifi with ADC2 on ESP32 workaround.

  int value = analogRead(this->pin);
  
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, this->wifi_register); // Wifi with ADC2 on ESP32 workaround.
  return value;
}

bool Joystick::left() {
    if (get_power() >= 0 && get_power() <= 1000)
        return true;
    return false;
}

bool Joystick::right() {
    if (get_power() >= 3000 && get_power() <= 4095)
        return true;
    return false;
}

bool Joystick::middle() {
    if (get_power() > 1000 && get_power() < 3000)
        return true;
    return false;
}
