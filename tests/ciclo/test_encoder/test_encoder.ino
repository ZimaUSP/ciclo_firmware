#include "Encoder.hpp"
#include "config.hpp"

#include "H_bridge_controller.hpp"
Encoder *encoder;
H_bridge_controller *BTS;
unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;

float current_position;
float last_position;
float delta_position;

int output;
float goal_vel;
float actual_vel;
float actual_rpm;

void setup() {     
  Serial.begin (9600);
  encoder = new Encoder(A_pin,B_pin,0,Nominal_pulses,pitch_pulley,1);
  encoder->init();
  last_t=millis();
  last_position=encoder->getPosition();
  BTS= new H_bridge_controller( r_pin, l_pin);
  BTS->init();
  BTS->Set_L(0);
  }
   
void loop() {
  current_t=millis();
  delta_t = current_t-last_t;
  
  
  if(delta_t>500){
    current_position=encoder->getPosition();
    delta_position= current_position - last_position;
    actual_vel=(delta_position/500);//meter/s
    actual_rpm=actual_vel*60/pitch_gear;
    last_t=current_t;
    last_position=current_position;
    Serial.println(actual_rpm); 
  }
  // PID_vel
  
  
  
  }
