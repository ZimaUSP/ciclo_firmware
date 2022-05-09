#include "current_sensor.hpp"
current_sensor *acs;
#define pin A2
void setup() {
  Serial.begin(9600);
  acs= new current_sensor(2,30);
}

void loop() {
  Serial.println(acs->get_current());
}
