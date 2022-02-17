#include "Encoder.hpp"
#include "config.hpp"

Encoder *encoder;

void setup() {     
  Serial.begin (9600);
  encoder = new Encoder(A_pin,B_pin,0,Nominal_pulses,pitch_pulley,1);
  encoder->init();
  }
   
void loop() {
  Serial.println(encoder->getPulses());
  Serial.println(encoder->getPosition());
  
  }
