/**
 * @file  Button.cpp
 *
 * @brief Button class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */


#include "Button.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

Button::Button(int pin, byte which):whichISR_(which) {
    this-> pin = pin;
    pinMode(this->pin,INPUT_PULLUP);
}
void Button::press_trigger() {
  if (digitalRead(this->pin)==LOW){
    this->press=true;
  }else{
    this->press=false;
  }
}

void Button::init() {
  switch (whichISR_) {
      case 0: 
        attachInterrupt (this->pin, isr0, CHANGE); 
        instance0_ = this;
        this->press_trigger();
        break;

      case 1: 
        attachInterrupt (this->pin, isr1, CHANGE); 
        instance1_ = this;
        this->press_trigger();
        break;

      case 2:
        attachInterrupt (this->pin, isr2, CHANGE); 
        instance2_ = this;
        this->press_trigger();
        break;
      
      case 3: 
        attachInterrupt (this->pin, isr3, CHANGE); 
        instance3_ = this;
        this->press_trigger();
        break;
    } 
}

int Button::getPin() {
 return this->pin;
}

bool Button::getPress() {
 return this->press;
}

// for use by ISR glue routines
Button * Button::instance0_;
Button * Button::instance1_;
Button * Button::instance2_;
Button * Button::instance3_;

void Button::isr0 (){
  instance0_->handleInterrupt();
}

void Button::isr1 (){
  instance1_->handleInterrupt();  
}

void Button::isr2 (){
  instance2_->handleInterrupt();  
}

void Button::isr3 (){
  instance3_->handleInterrupt();  
}

void Button::handleInterrupt(){
    this->press_trigger();
}