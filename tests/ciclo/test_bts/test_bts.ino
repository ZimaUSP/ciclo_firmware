
#include "H_bridge_controller.hpp"
#include "config.hpp"
H_bridge_controller *BTS;

void setup() {
  Serial.begin(9600);
  BTS= new H_bridge_controller( R_pin, L_pin);
  BTS->init();
}

void loop() {
  for(int i=0;i<MAX_PWM;i++){
      BTS->Set_R(i);
      delay(10);
  }

  for(int i=0; i<MAX_PWM;i++){
      BTS->Set_L(i);
      delay(10);
  }
}