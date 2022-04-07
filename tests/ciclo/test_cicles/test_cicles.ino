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

float current_position;
float last_position;
float delta_position;

int sample_t;
int current_cicles;
int last_cicles=0;
int delta_cicles;
int stand_by_cicles;
int passive_cicles;
int active_cicles;

float goal_vel;
float actual_vel;
float actual_rpm;
//PID constants
 
void setup() {    
  //this part is onli used to test cicle counter responsivity
  pinMode(8,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);

  Serial.begin (9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,perimeter_pulley,Mode);
  encoder->init();


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
  //integrate number of cicles on that state
  stand_by_cicles+=delta_cicles;

  // activate reles in the desired way
  passive_active_rele->turn_off();
  stand_by_active_rele->turn_off();
  return;
}

void active(){
  //integrate number of cicles on that state
  active_cicles +=delta_cicles;

  // activate reles in the desired way
  passive_active_rele->turn_off();
  stand_by_active_rele->turn_on();
  return;
}

// Call passive on loop to apply PID control to vel
void passive(){
  //integrate number of cicles on that state
  passive_cicles +=delta_cicles;

  // activate reles in the desired way
  passive_active_rele->turn_on();
  stand_by_active_rele->turn_off();
  //calculates PID values
  output = PID_vel->computePID(actual_vel,goal_vel);
  delay(5);
  // Setting direction of motion acording to output_ PID
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
    return;
}

void check_state(){
  // calculates RPM and number of cicles
  odometry_calc();
  
  /* 
  if (actual_rpm<goal_vel){
    STATE=PASSIVE;
    return;
  }

  PID_vel->reset();

  if(actual_rpm<(goal_vel+goal_vel)){
    STATE=STAND_BY;
  }else{
    STATE=ACTIVE;
  } */
  if (digitalRead(8)==LOW){
      STATE=PASSIVE;
      return;
  }
  if (digitalRead(4)==LOW){
    STATE=STAND_BY;
    return;
  }
  if (digitalRead(7)==LOW){
    STATE=ACTIVE;
    return;
  }
}

  // Reads the goal vel seted by the physiotherapist and calculates the actual vel
  void odometry_calc(){
    current_t=millis();
    delta_t = current_t-last_t;
   
    if(delta_t>sample_t){
      // Calculates postion variables
      current_position=encoder->getPosition();
      delta_position= current_position - last_position;
      last_position=current_position;

      //Calculates RPM using linear vel and pitch radius
      actual_vel=(delta_position/sample_t);//meter/s
      actual_rpm=actual_vel*60/pitch_gear;// RPM
      last_t=current_t;
Serial.print("actual_vel ");
  Serial.println(actual_vel);
      //calculates number of cicles that has been passed
      current_cicles = int(current_position/perimeter_gear); // calculates number of rotations. it only returns int values, thefore if a full cicle has not been done it wont count fractions. It is usefull to "integrate" cicles in each mode of exercise
      delta_cicles = current_cicles-last_cicles; // calculates if a cicle has been done or not. This is the variable that will be incremented in each function mode
      last_cicles = current_cicles;
            //debug print
      //Serial.println(actual_rpm); Serial.println(output); 
      
      // avoid overflowing variable pulses on encoder lib
      if (current_position>2700 || current_position<-2700){

        // Set pulses to 0 to avoid overflowing pulses variable
        encoder->setPulses(0);
        // Set last position and last pulses to 0 to avoid strange calculations when requirin vel,rpm and cicles counter
        last_position=0;
        last_cicles=0;
      }
      
    }

    goal_vel = 20; // metros a cada 100 segundos 
  
  }



