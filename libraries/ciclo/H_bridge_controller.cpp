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

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

H_bridge_controller::H_bridge_controller(int R_pin, int L_pin) {
    this-> R_pin = R_pin;
    this-> L_pin = L_pin;

}
void H_bridge_controller::init(){
  pinMode(this->R_pin, OUTPUT);

  pinMode(this->L_pin, OUTPUT);
}

void H_bridge_controller::SetPWM_R(int PWM) {
  analogWrite(this->R_pin, PWM);
}

void H_bridge_controller::SetPWM_L(int PWM) {
  analogWrite(this->L_pin, PWM);
}

void H_bridge_controller::Set_R(int PWM) {
  analogWrite(this->L_pin, 0);
  analogWrite(this->R_pin, PWM);
}

void H_bridge_controller::Set_L(int PWM) {
  analogWrite(this->R_pin, 0);
  analogWrite(this->L_pin, PWM);
}



