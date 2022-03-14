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

long current_pulses;
long last_pulses;
long delta_pulses;

int output;
float goal_vel;
float actual_vel;

//PID constants
 
void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(A_pin,B_pin,0,Nominal_pulses,pitch_pulley,Mode);
  encoder->init();

  BTS= new H_bridge_controller( R_pin, L_pin);
  BTS->init();

  PID_vel = new PID(kp,ki,kd);

  last_t=millis();
  last_pulses=encoder->getPulses();
  }
   
void loop() {
    current_t=millis();
    current_pulses=encoder->getPulses();

    delta_t = current_t-last_t;
    delta_pulses = current_pulses-last_pulses;
    
    last_t=millis();
    last_pulses=current_pulses;

    //goal_vel = map(analogRead(),1023,0,2);

    goal_vel=5; // metros a cada 100 segundos 

    // PID_vel

    actual_vel=int((delta_pulses/delta_t*Nominal_pulses*Mode)*60000L); // rpm
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
