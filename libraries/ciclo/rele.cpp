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

rele::rele(int pin,bool reverse_logic) {
    this->pin=pin;
    this->reverse_logic=reverse_logic;
}
void rele::init() {
  pinMode(this->pin,OUTPUT);
  return;
}
void rele::turn_on() {
  if(this->reverse_logic){
    digitalWrite(this->pin,LOW);
    return;
  }
  digitalWrite(this->pin,HIGH);
  return;
}
void rele::turn_off() {
  if(this->reverse_logic){
    digitalWrite(this->pin,HIGH);
    return;
  }
  digitalWrite(this->pin,LOW);
  return;
}