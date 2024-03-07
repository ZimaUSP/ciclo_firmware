
#include "config.hpp"

void setup() {
  Serial.begin(9600); 
}
void loop() {
  Serial.println(analogRead(pot_pin));
  
}