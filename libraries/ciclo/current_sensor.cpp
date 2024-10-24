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
  if(type==5){
    this->sensitivity= 0.185;
  }
  else if(type==20){
    this->sensitivity=0.095;
  }
  else if(type==30){
    this->sensitivity= 0.072;
  }
  
}

float current_sensor::get_current(){
  int sum=0; 
  for(int i=0; i<1000; i++){
    sum+=analogRead(this->pin);

  }
  return (float)((sum/1000)-512)*(5.000)/(1023.000*0.095);
}