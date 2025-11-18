/**
 * @file  H_bridge_controller.cpp
 *
 * @brief H_bridge_controller class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */


#include "H_bridge_controller.hpp"
#include <Arduino.h>

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

H_bridge_controller::H_bridge_controller(int R_pin, int L_pin, int PWM_frequency, int PWM_resolution, int r_channel, int l_channel) {
    this-> R_pin = R_pin;
    this-> L_pin = L_pin;
    this-> PWM_frequency = PWM_frequency;
    this-> PWM_resolution = PWM_resolution;
    this-> r_channel = r_channel;
    this-> l_channel = l_channel;

}
void H_bridge_controller::init(){
  ledcAttachPin(R_pin, this->r_channel);
  ledcSetup(this->r_channel, PWM_frequency, PWM_resolution);

  ledcAttachPin(L_pin, this->l_channel);
  ledcSetup(this->l_channel, PWM_frequency, PWM_resolution);
}

void H_bridge_controller::SetPWM_R(int PWM) {
  ledcWrite(this->r_channel, PWM);
}

void H_bridge_controller::SetPWM_L(int PWM) {
  ledcWrite(this->l_channel, PWM);
}

void H_bridge_controller::Set_R(int PWM) {
  ledcWrite(this->l_channel, 0);
  ledcWrite(this->r_channel, PWM);
}

void H_bridge_controller::Set_L(int PWM) {
  ledcWrite(this->r_channel, 0);
  ledcWrite(this->l_channel, PWM);
}



