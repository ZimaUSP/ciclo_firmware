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
current_sensor::current_sensor(uint8_t pin,uint8_t type){
  this->pin=pin;
  this->type=type;
  if(type==A05){
    this->sensitivity= 0.185;
  }
  else if(type==A20){
    this->sensitivity= 0.100;
  }
  else if(type==A30){
    this->sensitivity= 072;
  }
  
}

float current_sensor::get_current(){
  long sum=0;

  for(int i=0; i<1000; i++){
    sum+=analogRead(this->pin);
  }

  return (float)(sum/1000-512)*(5.000)/(1023.000*this->sensitivity);
}
