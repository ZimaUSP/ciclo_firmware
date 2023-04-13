
#include "H_bridge_controller.hpp"
#include "config.hpp"
H_bridge_controller *BTS;

void setup() {
  Serial.begin(9600);
  BTS = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, 0, 1);
  BTS->init();
}

void loop() {
  
BTS->Set_R(map(analogRead(pot_pin),0,4095,0,255)); //Set_l = mov resistido , Set_R = mov assistido
Serial.println(map(analogRead(pot_pin),0,4095,0,255));

}