/**
 * @file  Timer.cpp
 *
 * @brief Encoder class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 03/2022
 * 
 * @copyright MIT License
 */


#include "Timer.hpp"
/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

Timer::Timer(){
  this->pause_time=pause_time;
  this->unpause_time=unpause_time;
  this->paused= paused;
}

void Timer::init(int period) {
  this->period = period*60000UL;
  this->init_t = millis();
}


int  Timer::current_min() {
  if (paused){
    return int(this->passed_time/60000UL);
  }
  if (millis() >this->period){
    return 0;
  }
  this->passed_time = this->period - (millis() - this->init_t );
  return int(this->passed_time/60000UL);
}

int  Timer::current_sec() {
  if (paused){
    return int(this->passed_time/1000UL - this->current_min()*60UL);
  }
  if (millis() >this->period){
    return 0;
  }
  this->passed_time = this->period - (millis() - this->init_t);
  return int(this->passed_time/1000UL - this->current_min()*60UL);
}

void Timer::pause() {
  this->paused=true;
  return;
}

void Timer::unpause() {
  this->period=this->passed_time;
  this->init_t=millis();
  this->paused=false;
  return;
}
