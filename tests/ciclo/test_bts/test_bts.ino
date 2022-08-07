
#include "H_bridge_controller.hpp"
#include "config.hpp"
H_bridge_controller *Motor;

void setup() {
  Serial.begin(9600);
  Motor= new H_bridge_controller( r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  Motor->init();
}

void loop() {
  for(int i=0;i<1024;i++){
      Motor->Set_R(i);
      delay(10);
  }

  for(int i=0; i<1024;i++){
      Motor->Set_L(i);
      delay(10);
  }
}