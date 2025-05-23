
#include "H_bridge_controller.hpp"
#include "config.hpp"
H_bridge_controller *BTS;

void setup() {
  Serial.begin(9600);7
  BTS = new H_bridge_controller(r_pin, 32, PWM_frequency_channel, PWM_resolution_channel, 0, 1);
  BTS->init();
  Serial.println("i am here");
  delay(1000);
}

void loop() {
  
  Serial.println("go right");
  BTS->Set_R(40); //Set_l = mov resistido , Set_R = mov assistido
  delay(1000);
  Serial.println("stop");
  BTS->Set_R(0); //Set_l = mov resistido , Set_R = mov assistido
  delay(1000);
  Serial.println("go left");
  BTS->Set_L(40); //Set_l = mov resistido , Set_R = mov assistido
  delay(1000);
  Serial.println("go stop");
  BTS->Set_L(0); //Set_l = mov resistido , Set_R = mov assistido
  delay(1000);

}