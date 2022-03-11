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
}

void Timer::init(int period) {
  this->period = period*60*1000;
  this->init_t = millis();
}

int  Timer::current_min() {

  this->passed_time = this->period - (millis() - this->init_t - (this->unpause_time - this->pause_time));
  return int(this->passed_time/60*1000);
}

int  Timer::current_sec() {
  this->passed_time = this->period - (millis() - this->init_t - (this->unpause_time - this->pause_time));
  return int(this->passed_time/1000 - this->current_min()*60);
}

void Timer::pause() {
  this->pause_time = millis();
  return;
}

void Timer::unpause() {
  this->unpause_time = millis();
  return;
}
