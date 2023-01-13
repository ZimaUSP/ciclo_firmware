#include "current_sensor.hpp"
#include "H_bridge_controller.hpp"
#include "config.hpp"
H_bridge_controller *Motor;
current_sensor *acs;


void setup() {
  Serial.begin(9600);
  Motor= new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  Motor->init();
  
  acs= new current_sensor(acs_pin,20);

}

void loop() {
  for(int i=0;i<255;i++){
      
      Motor->Set_R(i);
      Serial.println(acs->get_current());

      delay(100);
  }

}


