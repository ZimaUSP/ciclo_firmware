#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "PID.hpp"
#include "config.hpp"

Encoder *encoder;
H_bridge_controller *BTS;

PID *PID_vel; 
unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;

double current_position;
double last_position;
double delta_position;

int output;
float goal_vel;
float actual_vel;
float actual_rpm;

 
void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,pitch_pulley,Mode);
  encoder->init();

  BTS= new H_bridge_controller( r_pin, l_pin);
  BTS->init();

  PID_vel = new PID(kp,ki,kd);

  last_t=millis();
  last_position=encoder->getPosition();
  }
   
void loop() {
    current_t=millis();
    delta_t = current_t-last_t;
    
    
    if(delta_t>200){
      current_position=encoder->getPosition();
      delta_position= current_position - last_position;
      actual_vel=(delta_position/200);//meter/s
      actual_rpm=actual_vel*60/pitch_gear;// RPM
      last_t=current_t;
      last_position=current_position;
      Serial.println(actual_rpm); 
      Serial.println(output); 
      if (current_position>10000 || current_position<-10000){
        encoder->setPulses(0);
        last_position=0;
      }
      
    }
    goal_vel=-20; // metros a cada 100 segundos 

    // PID_vel

    output = PID_vel->computePID(actual_rpm,goal_vel);
    delay(5);

    // Setting direction of motion acording to output_x PID
    if (output < 0) {
        if (output < -MAX_PWM) {
          output = -MAX_PWM;
        }
        BTS->Set_R(-output);
        return;
      } else {
        if (output > MAX_PWM) {
          output = MAX_PWM;
        }
        BTS->Set_L(output);
        return;
      }

  }
