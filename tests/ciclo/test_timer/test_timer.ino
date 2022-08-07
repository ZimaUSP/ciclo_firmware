
#include "Timer.hpp"
Timer *LCD_timer;
int entered=0;
unsigned long last_time;
void setup() {
  Serial.begin(9600);
  LCD_timer= new Timer();
  LCD_timer->init(1);
}

void loop() {
  Serial.print(LCD_timer->current_min());
  Serial.print(":");
  Serial.println(LCD_timer->current_sec());

}