// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"


//STATE
char STATE = 0 ; 


#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "PID.hpp"
#include "rele.hpp"

Encoder *encoder;
H_bridge_controller *BTS;
rele *passive_active_rele; // normaly open ->active normaly // normaly close passive
rele *stand_by_active_rele; // normaly open-> stand_by // normaly close -> active

PID *PID_vel; 
double current_t;
double last_t;
double delta_t;a

float current_position;
float last_position;
float delta_position;

int current_cicles;
int last_cicles=0;
int delta_cicles;
int stand_by_cicles;
int passive_cicles;
int active_cicles;

int output;
float goal_vel;
float actual_vel;

//PID constants
 
void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,pitch_pulley,Mode);
  encoder->init();

  BTS= new H_bridge_controller( r_pin, l_pin);
  BTS->init();

  passive_active_rele= new rele(2);
  passive_active_rele->init();

  stand_by_active_rele= new rele(3);
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
    //integrates number of cicles on that mode
    stand_by_cicles+=delta_cicles;

    // switches the right path to let no resistance nor assistance
    passive_active_rele->turn_off();
    stand_by_active_rele->turn_off();
    return;
  }

  void active(){
    //integrates number of cicles on that mode
    active_cicles+=delta_cicles;

    // switches the right path to give resistive mode
    passive_active_rele->turn_off();
    stand_by_active_rele->turn_on();
    return;
  }

  // Call passive on loop to apply PID control to vel
  void passive(){
    //integrates number of cicles on that mode
    passive_cicles+=delta_cicles;

    // switches the right path to give passive mode
    passive_active_rele->turn_on();
    stand_by_active_rele->turn_off();

    // computes PID outpu based on RPM on actual vel comparing to GOAL vel
    output = PID_vel->computePID(actual_vel,goal_vel);

    // Setting direction of motion acording to output PID
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

// compares actual vel to referecen vel seted by the phisiotherapist and than choses the right state
void check_state(){
  read_vel_cicles();
  if (actual_vel<goal_vel){
   STATE=PASSIVE;
  }else if(actual_vel<goal_vel*1.1){
    STATE=STAND_BY;
  }else{
    STATE=ACTIVE;
  }  
}

// Reads the goal vel seted by the physiotherapist and calculates the actual vel
void read_vel_cicles(){
  // calculates times variables
  current_t=millis();
  delta_t = current_t-last_t;
  last_t=current_t;

  // calculates positions varibles
  current_position=encoder->getPosition();
  delta_position = current_position-last_position;
  last_position=current_position;

  //calculates number of cicles that has been passed
  current_cicles = int(current_position/Mode*Nominal_pulses);
  delta_cicles = current_cicles-last_cicles;
  last_cicles = current_cicles;

  //goal_vel = map(analogRead(),1023,0,2);
  goal_vel=50; // RPM
  // PID_vel
  actual_vel=int((delta_position/delta_t*pitch_gear)*60); // Pegando apenas as 2 primeiras casas decimais RPM

}