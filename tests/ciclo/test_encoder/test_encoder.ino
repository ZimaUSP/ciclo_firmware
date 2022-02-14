#include "Encoder.hpp"
#include "config.hpp"
int A_pin=2; // green cable
int B_pin=3; // white cable

Encoder *encoder;

void setup() {     
  Serial.begin (9600);
  encoder = new Encoder(A_pin,B_pin,0,Nominal_pulses,pitch_pulley,4);
  encoder->init();
  }
   
void loop() {
  Serial.println(encoder->getPulses());
  }

