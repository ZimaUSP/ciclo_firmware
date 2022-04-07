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

unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;

double current_position;
double last_position;
double delta_position;

float goal_vel;
float actual_vel;
float actual_rpm;
//PID constants
 
void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,pitch_pulley,Mode);
  encoder->init();

  BTS= new H_bridge_controller(r_pin, l_pin);
  BTS->init();

  passive_active_rele= new rele(9,false); // Este rele em especifico tem logica reversa
  passive_active_rele->init();

  stand_by_active_rele= new rele(10,true);
  stand_by_active_rele->init();

  PID_vel = new PID(kp,ki,kd);

  last_t=millis();
  last_position=encoder->getPosition();
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
  delay(5);
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
  if (actual_rpm<goal_vel){
    STATE=PASSIVE;
    return;
  }
  PID_vel->reset();

  Serial.println(actual_rpm);
  if(actual_rpm<(goal_vel+goal_vel)){
    STATE=STAND_BY;
  }else{
    STATE=ACTIVE;
  }  
}

  // Reads the goal vel seted by the physiotherapist and calculates the actual vel
  void read_rpm(){
    current_t=millis();
    delta_t = current_t-last_t;
    
    if(delta_t>sample_t){
      current_position=encoder->getPosition();
      delta_position= current_position - last_position;
      last_position=current_position;

      actual_vel=(delta_position/sample_t);//meter/s
      actual_rpm=actual_vel*60/pitch_gear;// RPM
      last_t=current_t;

      //debug print
      //Serial.println(actual_rpm); Serial.println(output); 
      
      // avoid overflowing variable pulses on encoder lib
      if (current_position>10000 || current_position<-10000){
        encoder->setPulses(0);
        last_position=0;
      }
      
    }

    goal_vel = 20; // metros a cada 100 segundos 
    return;
  }