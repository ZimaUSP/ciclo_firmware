#include "Encoder.hpp"
#include "config.hpp"

#include "H_bridge_controller.hpp"
Encoder *encoder;
H_bridge_controller *Motor;

unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;

float current_position;
float last_position;
float delta_position;

float actual_vel;
float actual_rpm;

void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,perimeter_pulley,Mode);
  encoder->init();

  Motor= new H_bridge_controller( r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  Motor->init();
  Motor->Set_L(0);

  last_t=millis();
  last_position=encoder->getPosition();
  }
   
void loop() {
  current_t=millis();
  delta_t = current_t-last_t;  
  if(delta_t>10){
    current_position=encoder->getPosition();
    delta_position= current_position - last_position;

    actual_vel=(delta_position/delta_t);//meter/s
    actual_rpm=actual_vel*60/pitch_gear;

    last_t=current_t;
    last_position=current_position;
    
    Serial.println(actual_rpm); 
  }
  }
