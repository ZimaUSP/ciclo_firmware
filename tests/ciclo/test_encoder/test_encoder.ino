#include "Encoder.hpp"

//#include "H_bridge_controller.hpp"
#include "config.hpp"
Encoder *encoder;

//H_bridge_controller *Motor;

unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;

float current_position;
float last_position;
float delta_position;

float actual_vel;
float actual_rpm;

double current_pulses;
double delta_pulses;
double last_pulses;
double revolutions;




void setup() {     
  Serial.begin(9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,Mode);
  encoder->init();

  //Motor= new H_bridge_controller( r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  //Motor->init();
  //Motor->Set_L(80);

  last_t=millis();
  }
   
void loop() {
  //Sentido assistido posição negativa

 // Motor->Set_L(0);
  current_t=millis();
  delta_t = current_t-last_t;  
  //Serial.print("pulses: ");
  //Serial.println(encoder->getPulses()); 

  if (delta_t > sample_t) {
    current_pulses = encoder->getPulses();
    delta_pulses = current_pulses - last_pulses;
    revolutions = delta_pulses/pulses_per_rev;
    actual_rpm = revolutions*(60000/delta_t);

    Serial.println("rpm");
    Serial.println(abs(actual_rpm));

    last_t = current_t;
    last_pulses = current_pulses;

    

    //Serial.println("vel");
    //Serial.println(actual_vel); 

  }
}
  
