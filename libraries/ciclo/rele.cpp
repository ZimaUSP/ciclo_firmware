/**
 * @file  rele.cpp
 *
 * @brief rele class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 02/2022
 * 
 * @copyright MIT License
 */


#include "rele.hpp"
/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

rele::rele(int pin) {
    this->pin=pin;
}
void rele::init() {
  pinMode(this->pin,OUTPUT);
  return;
}
void rele::turn_on() {
  digitalWrite(this->pin,HIGH);
  return;
}
void rele::turn_off() {
  digitalWrite(this->pin,LOW);
  return;
}