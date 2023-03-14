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
float goal_rpm;

float actual_vel;
float actual_rpm;
int a,b,c,d,e;
int count =0;
 
void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,perimeter_pulley,Mode);
  encoder->init();

  BTS = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  BTS->init();

  PID_vel = new PID(kp,ki,kd,i_saturation);

  last_t=millis();
  last_position=encoder->getPosition();
  }
   
void loop() {
    current_t=millis();
    delta_t = current_t-last_t;

    if(delta_t>2000){
      current_position = encoder->getPosition();
      delta_position = current_position - last_position;
      actual_vel=(delta_position/2000);//meter/s
      actual_rpm=actual_vel*60/pitch_gear;// RPM
      last_t=current_t;
      last_position=current_position;

      Serial.print("pos: ");
      Serial.print(delta_position); 
      Serial.print(",");
      Serial.print("vel: ");
      Serial.println(actual_vel);
      
      if (current_position>10000 || current_position<-10000){ // zerar encoder para não 
        encoder->setPulses(0);
        last_position=0;
      }
      
    }

    goal_rpm = -20; // rotações a cada 2000 milisegundos 

    // PID_vel

    output = PID_vel->computePID(actual_rpm,goal_rpm,tolerance);
    
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
