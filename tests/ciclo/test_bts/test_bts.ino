
#include "H_bridge_controller.hpp"
#include "config.hpp"
H_bridge_controller *BTS;

void setup() {
  Serial.begin(9600);
  BTS = new H_bridge_controller(r_pin, 32, PWM_frequency_channel, PWM_resolution_channel, 0, 1);
  BTS->init();
}

void loop() {
  
BTS->Set_R(50); //Set_l = mov resistido , Set_R = mov assistido
delay(1000);
BTS->Set_R(0); //Set_l = mov resistido , Set_R = mov assistido
delay(1000);
BTS->Set_L(50); //Set_l = mov resistido , Set_R = mov assistido
delay(1000);
BTS->Set_L(0); //Set_l = mov resistido , Set_R = mov assistido
delay(1000);

}