
#include "config.hpp"
#include "soc/sens_reg.h" // needed for manipulating ADC2 control register
uint64_t reg_b; // Used to store ADC2 control register
#define PIN 15 // Substitute xx with your ADC2 Pin number
int value;

void setup() {
  Serial.begin(9600); 
}
void loop() {
  SET_PERI_REG_MASK(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_DATA_INV);
  //We have to do the 2 previous instructions BEFORE EVERY analogRead() calling!
  value = analogRead(PIN);
  Serial.println(value);
  delay(2);//allow the cpu to switch to other tasks
  Serial.println(value);
  
}