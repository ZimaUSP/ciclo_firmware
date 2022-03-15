// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"

//STATE
char STATE = 0 ; 

// including libs
#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "PID.hpp"
#include "rele.hpp"

Encoder *encoder;
H_bridge_controller *BTS;
rele *passive_active_rele; // normaly open ->active normaly // normaly close passive
rele *stand_by_active_rele; // normaly open-> stand_by // normaly close -> active

PID *PID_vel;
int output;

double current_t;
double last_t;
double delta_t;

long current_pulses;
long last_pulses;
long delta_pulses;


float goal_vel;
float actual_vel;

//PID constants
 
void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(A_pin,B_pin,0,Nominal_pulses,pitch_pulley,Mode);
  encoder->init();

  BTS= new H_bridge_controller( R_pin, L_pin);
  BTS->init();

  passive_active_rele= new rele(2);
  passive_active_rele->init();

  stand_by_active_rele= new rele(3);
  stand_by_active_rele->init();

  PID_vel = new PID(kp,ki,kd);

  last_t=millis();
  last_pulses=encoder->getPulses();
  }
   
void loop() {
    switch(STATE) {
        case STAND_BY :        
          check_state();
          // Niether passive nor active assistence is given now
          stand_by();
          return;
        case ACTIVE :        
          check_state();
          //Now the patient is the one who moves the motor within resistance
          active();
          return;
        case PASSIVE :        
          check_state();
          //The motor is activeted to help patients move.
          passive();
          return;
    }  
  }
  void stand_by(){
    passive_active_rele->turn_off();
    stand_by_active_rele->turn_off();
    return;
  }

  void active(){
    passive_active_rele->turn_off();
    stand_by_active_rele->turn_on();
    return;
  }

  // Call passive on loop to apply PID control to vel
  void passive(){
    passive_active_rele->turn_on();
    stand_by_active_rele->turn_off();
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

void check_state(){
  read_rpm();
  if (actual_vel<goal_vel){
    STATE=PASSIVE;
    return;
  }
  PID_vel->reset();
  if(actual_vel<goal_vel*1.1){
    STATE=STAND_BY;
  }else{
    STATE=ACTIVE;
  }  
}

  // Reads the goal vel seted by the physiotherapist and calculates the actual vel
  void read_rpm(){
    current_t = millis();
    delta_t = current_t-last_t;
    last_t = current_t;

    current_pulses = encoder->getPulses();
    delta_pulses = current_pulses-last_pulses;
    last_pulses = current_pulses;
    
    //goal_vel = map(analogRead(),1023,0,2);

    goal_vel = 5; // metros a cada 100 segundos 

    // PID_vel
    actual_vel = int((delta_pulses/delta_t*Nominal_pulses*Mode)*60000L); // rpm
    return;
  }