
#include "Button.hpp"
#include "config.hpp"
Button *btn;

void setup() {
  Serial.begin(9600);
  btn= new Button(btn_pin,0);
  btn->init();
}

void loop() {
  if(btn->getPress()){
    Serial.println("button pressed");
  }else{
    Serial.println("button not pressed");
  }
}