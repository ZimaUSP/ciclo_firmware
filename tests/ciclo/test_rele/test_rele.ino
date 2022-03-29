#include "rele.hpp"

#include "H_bridge_controller.hpp"
rele *Rele;

void setup() {     
  Serial.begin (9600);
  Rele= new rele(9);
  Rele->init();
  }
   
void loop() {

  Rele->turn_off();
  delay(1000);
  }
