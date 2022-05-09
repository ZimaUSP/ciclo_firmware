// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"

//STATE
char STATE = 0 ; 

// including libs
#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "PID.hpp"
#include "rele.hpp"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

Encoder *encoder;
H_bridge_controller *BTS;
rele *passive_active_rele; // normaly open ->active normaly // normaly close passive
rele *stand_by_active_rele; // normaly open-> stand_by // normaly close -> active
PID *PID_vel;
LiquidCrystal_I2C lcd(0x27,16,2); // LCD lib

int output;


unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;

double current_position;
double last_position;
double delta_position;

float goal_vel=25;
float actual_vel;
float actual_rpm;

int current_cicles;
int last_cicles=0;
int delta_cicles;
int stand_by_cicles;
int passive_cicles;
int active_cicles;
 
int state_mode;

void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,perimeter_pulley,Mode);
  encoder->init();

  BTS= new H_bridge_controller(r_pin, l_pin);
  BTS->init();

  passive_active_rele= new rele(passive_active,true); // Este rele em especifico tem logica reversa
  passive_active_rele->init();

  stand_by_active_rele= new rele(stand_by_active,true);
  stand_by_active_rele->init();

  PID_vel = new PID(kp,ki,kd);

  lcd.init();                      // initialize the lcd 
  last_t=millis();


  last_position=encoder->getPosition();

  
}
   
void loop() {
    printLCD();
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
        case FADE :         
          check_state();
          //The motor is activeted to help patients move.
          fade_pwm();
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
  delta_cicles=0;

  // activate reles in the desired way
  passive_active_rele->turn_off();
  stand_by_active_rele->turn_off();
  return;
}

void active(){
  //integrate number of cicles on that state
  active_cicles +=delta_cicles;
  delta_cicles=0;

  // activate reles in the desired way
  passive_active_rele->turn_off();
  stand_by_active_rele->turn_on();
  return;
}

void fade_pwm(){
  STATE=PASSIVE;
  //integrate number of cicles on that state
  passive_cicles +=delta_cicles;
  delta_cicles=0;

  // activate reles in the desired way
  passive_active_rele->turn_on();
  stand_by_active_rele->turn_on();

 // map it to the range of the analog out:
  output = PID_vel->computePID(actual_vel*100,goal_vel); // Don't know why but input is divided by 100 on Compute PID function
 
  if (output < 0) {
      if (output < -MAX_PWM) {
        output = -MAX_PWM;
      }
      for(int i=0;i<output;i++){
        BTS->Set_L(-i/10);
        delay(fade);
      }
      return;
    } else {
      if (output > MAX_PWM) {
        output = MAX_PWM;
      }
      for(int i=0;i<output;i++){
        BTS->Set_R(i);
        delay(fade);
      }
      return;
    }
}
// Call passive on loop to apply PID control to vel
void passive(){
  //integrate number of cicles on that state
  passive_cicles +=delta_cicles;
  delta_cicles=0;

  // activate reles in the desired way
  passive_active_rele->turn_on();
  stand_by_active_rele->turn_on();

 // map it to the range of the analog out:
  output = PID_vel->computePID(actual_vel*100,goal_vel); // Don't know why but input is divided by 100 on Compute PID function
  
  //debug print
  //Serial.print(actual_rpm);Serial.print(" ");Serial.println(goal_vel);
  //delay(5);
  // Setting direction of motion acording to output_x PID
  if (output < 0) {
      if (output < -MAX_PWM) {
        output = -MAX_PWM;
      }
      BTS->Set_L(-output/10);
      return;
    } else {
      if (output > MAX_PWM) {
        output = MAX_PWM;
      }
      BTS->Set_R(output);
      return;
    }
}

void check_state(){
  odometry_calc();
  state_mode = float(map(analogRead(pot_pin), 0, 1023, 0, 2));
  Serial.print(analogRead(pot_pin));
  Serial.print(analogRead(" "));
  Serial.println(state_mode);
  if (state_mode==PASSIVE){
    STATE=PASSIVE;
    return;
  }
  PID_vel->reset();

  if(state_mode==(STAND_BY)){
    STATE=STAND_BY;
  }else{
    STATE=ACTIVE;
  }  
}

  // Reads the goal vel seted by the physiotherapist and calculates the actual vel
  void odometry_calc(){
    current_t=millis();
    delta_t = current_t-last_t;
    
    //debug print
    //Serial.println(delta_t);
    if(delta_t>sample_t){
      
      // Calculates postion variables
      current_position=encoder->getPosition();
      delta_position= current_position - last_position;
      last_position=current_position;
      
      //Calculates RPM using linear vel and pitch radius
      actual_vel=(delta_position/sample_t);//meter/s
      actual_rpm=actual_vel*60/pitch_gear;// RPM
      last_t=current_t;

      //calculates number of cicles that has been passed
      current_cicles = int(current_position/perimeter_gear); // calculates number of rotations. it only returns int values, thefore if a full cicle has not been done it wont count fractions. It is usefull to "integrate" cicles in each mode of exercise
      delta_cicles = current_cicles-last_cicles; // calculates if a cicle has been done or not. This is the variable that will be incremented in each function mode
      last_cicles = current_cicles;
      
      
      // avoid overflowing variable pulses on encoder lib
      if (current_position>10000 || current_position<-10000){
        encoder->setPulses(0);
        last_position=0;
        last_cicles=0;
      }
     
    
    }
    return;
  }

  void printLCD(){
    lcd.backlight();
    lcd.setCursor(0,0);
    if (STATE==STAND_BY){
      lcd.print("Modo: Normal   ");
    }else if (STATE==ACTIVE){
      lcd.print("Modo: Resistido");
    }else if (STATE==PASSIVE){
      lcd.print("Modo: Assistido");
    }
    
   
    lcd.setCursor(0,1);
    lcd.print("Ciclos: ");
    lcd.print(active_cicles+passive_cicles+stand_by_cicles);
    //Serial.print("Ciclos: ");Serial.print(active_cicles); Serial.print(", "); Serial.print(passive_cicles); Serial.print(", "); Serial.println(stand_by_cicles);
    
  }