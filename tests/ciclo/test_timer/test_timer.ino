
#include "Timer.hpp"
Timer *LCD_timer;
int entered=0
void setup() {
  Serial.begin(9600);
  LCD_timer= new Timer();
  LCD_timer->init(10);
}

void loop() {
  Serial.print(LCD_timer->current_min);
  Serial.print(":");
  Serial.println(LCD_timer->current_sec);
  if(Serial.available()){
    if(entered){
      LCD_timer->unpause();
    }
    LCD_timer->pause();
  }
}