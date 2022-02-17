#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "PID.hpp"
#include "config.hpp"

Encoder *encoder;
H_bridge_controller *BTS;

PID *PID_vel; 
double current_t;
double last_t;
double delta_t;
float current_position;
float last_position;
float delta_position;
int output;
float goal_vel;
float actual_vel;
int actual_vel_100x;

//PID constants
 
void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(A_pin,B_pin,0,Nominal_pulses,pitch_pulley,Mode);
  encoder->init();

  BTS= new H_bridge_controller( R_pin, L_pin);
  BTS->init();

  PID_vel = new PID(kp,ki,kd);
  last_t=millis();
  last_position=encoder->getPosition();
  }
   
void loop() {
    current_t=millis();
    current_position=encoder->getPosition();

    delta_t = current_t-last_t;
    delta_position = current_position-last_position;
    
    last_t=millis();
    last_position=encoder->getPosition();

    //goal_vel = map(analogRead(),1023,0,2);
    goal_vel=50; // metros a cada 100 segundos 
    // PID_vel
    actual_vel=int((delta_position/delta_t)*100); // Pegando apenas as 2 primeiras casas decimais Metros /100 Segundos

    
    output = PID_vel->computePID(actual_vel,goal_vel);
    // Setting direction of motion acording to output_x PID
    if (output < 0) {
        if (output < -MAX_PWM) {
          output = -MAX_PWM;
        }
        //BTS->Set_R(-output);
        return;
      } else {
        if (output > MAX_PWM) {
          output = MAX_PWM;
        }
        BTS->Set_L(output);
        return;
      }

  }
