// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"

//STATE
char STATE = MODE; 
char LAST_STATE = MODE ;
int modo = AUTO;
char HEALTH_STATE;

// including libs
#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "PID.hpp"
#include "rele.hpp"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Timer.hpp"

Encoder *encoder;
H_bridge_controller *BTS;
rele *passive_active_rele; // normaly open ->active normaly // normaly close passive
rele *stand_by_active_rele; // normaly open-> stand_by // normaly close -> active
PID *PID_vel;
LiquidCrystal_I2C lcd(0x27,16,2); // LCD lib
Timer *clock;
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
int duration;
int state_mode;
int health_p;
char time[2];
char cicles[4];
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


  clock= new Timer(); 
  lcd.init();                      // initialize the lcd 
  last_t=millis();
  
  pinMode(btn_pin,INPUT_PULLUP);

  last_position=encoder->getPosition();

  
}
   
void loop() {
    printLCD();
    switch(STATE) {
        case STAND_BY :
          LAST_STATE = STAND_BY;    
          check_state();
          // Niether passive nor active assistence is given now
          stand_by();
          return;
        case ACTIVE_PLUS :
          LAST_STATE = ACTIVE_PLUS;          
          check_state();
          //Now the patient is the one who moves the motor within resistance
          active_plus();
          return;
        case FADE :
          LAST_STATE = PASSIVE;
          //The motor is activeted GRADUALLY to help patients move.
          fade_pwm();
          return;
        case PASSIVE :
          LAST_STATE = PASSIVE;         
          check_state();
          //The motor is activeted to help patients move.
          passive();
          return;
        case MODE :
          LAST_STATE = MODE; 
          //The motor is activeted to help patients move.
          mode();
          return;
        case TIMER :
          LAST_STATE = TIMER; 
          //ask duration 
          timer();
          return;
        case DONE :
          LAST_STATE = DONE; 
          //show results
          done();
          return;
        case RESET :
          LAST_STATE = RESET; 
          //reset machine
          reset();
          return;
    }  
}

void stand_by(){
  BTS->Set_L(0);
  //integrate number of cicles on that state
  stand_by_cicles+=delta_cicles;
  delta_cicles=0;

  // activate reles in the desired way
  passive_active_rele->turn_off();
  stand_by_active_rele->turn_off();
  return;
}

void active(){
  BTS->Set_L(0);
  //integrate number of cicles on that state
  active_cicles +=delta_cicles;
  delta_cicles=0;

  // activate reles in the desired way
  passive_active_rele->turn_off();
  stand_by_active_rele->turn_on();
  return;
}

void active_plus(){
  //integrate number of cicles on that state
  active_cicles +=delta_cicles;
  delta_cicles=0;

   // activate reles in the desired way
  passive_active_rele->turn_on();
  stand_by_active_rele->turn_on();

  // map it to the range of the analog out:
  output = PID_vel->computePID(actual_vel*100,0); // Don't know why but input is divided by 100 on Compute PID function
 
  if (output < 0) {
      if (output < -MAX_PWM) {
        output = -MAX_PWM;
      }
      BTS->Set_L(output);
      delay(fade);
      return;
    } else {
      BTS->Set_R(0);
      return;
    }
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
  output = PID_vel->computePID(actual_vel*100,goal_vel)/2; // Don't know why but input is divided by 100 on Compute PID function
 
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
        BTS->Set_L(-i/10);
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
      BTS->Set_L(0);
      return;
    } else {
      if (output > MAX_PWM) {
        output = MAX_PWM;
      }
      BTS->Set_R(output);
      return;
    }
}
void mode(){
  if(digitalRead(btn_pin)==LOW){ // verifica se apertou botão de start enquanto isso vai printando a duração do ex
    Serial.print(modo);
    STATE=TIMER;
    delay(300);
    return;
  }
  modo=map(analogRead(pot_pin),0,1023,0,1);

  return;
}
void timer(){
  
  if(digitalRead(btn_pin)==LOW){ // verifica se apertou botão de start enquanto isso vai printando a duração do ex
    Serial.print("START");
    STATE=STAND_BY;
    clock->init(duration);
    return;
  }
  duration=map(analogRead(pot_pin),0,1023,0,30);
  
  return;
}
void done(){
  BTS->Set_L(0);
  if(digitalRead(btn_pin)==LOW){ // verifica se apertou botão de reiniciar  e reseta as variaveis
    STATE=RESET;
    int stand_by_cicles=0;
    int passive_cicles=0;
    int active_cicles=0;
  }
  return;
}
void reset(){
  delay(2000);
  RESET_CMD; 
  return;
}
void printLCD(){
  lcd.backlight();
  lcd.setCursor(0,0);
  if (STATE==STAND_BY){
    lcd.print("Modo: Normal   ");
    lcd.setCursor(0,1);
    lcd.print("               ");
    lcd.setCursor(0,1);
    sprintf(time,"%02d",clock->current_min());
    lcd.print(time);
    lcd.print(":");
    sprintf(time,"%02d",clock->current_sec());
    lcd.print(time);
  }else if (STATE==ACTIVE_PLUS){
    lcd.print("Modo: Resistido");
    lcd.setCursor(0,1);
    lcd.print("               ");
    lcd.setCursor(0,1);
    sprintf(time,"%02d",clock->current_min());
    lcd.print(time);
    lcd.print(":");
    sprintf(time,"%02d",clock->current_sec());
    lcd.print(time);
  }else if (STATE==PASSIVE|| STATE==FADE){
    lcd.print("Modo: Assistido");
    lcd.setCursor(0,1);
    lcd.print("               ");
    lcd.setCursor(0,1);
    sprintf(time,"%02d",clock->current_min());
    lcd.print(time);
    lcd.print(":");
    sprintf(time,"%02d",clock->current_sec());
    lcd.print(time);
  }else if(STATE==MODE){
    lcd.print("Escolha o modo ");
    lcd.setCursor(0,1);
    lcd.print("               ");
    lcd.setCursor(0,1);
    if(modo==AUTO){
      lcd.print("  Automatico   ");
    }else if(modo==MANUAL){
      lcd.print("    Manual    ");
    }
  }else if(STATE==TIMER){
    lcd.print("Duracao do Ex :");
    lcd.setCursor(0,1);
    lcd.print("               ");
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.print(duration);
    lcd.print(":00");
  }else if(STATE==DONE){
    lcd.print(" -  |  0  |  + ");
    lcd.setCursor(0,1);
    sprintf(cicles,"%04d",passive_cicles);
    lcd.print(cicles);
    lcd.print(" ");
    sprintf(cicles,"%04d",stand_by_cicles);
    lcd.print(cicles);
    lcd.print(" ");
    sprintf(cicles,"%04d",active_cicles);
    lcd.print(cicles);
  }
  else if(STATE==RESET){
    lcd.print("Reiniciando... ");
    lcd.setCursor(0,1);
    lcd.print("               ");
    
  }
}

void check_state(){
  if(clock->current_min()==0 && clock->current_sec()==0){
    STATE=DONE;
    return;
  }
  if(modo==MANUAL){
    state_mode = float(map(analogRead(pot_pin), 0, 1023, 0, 2));
    Serial.print(analogRead(pot_pin));
    Serial.print(analogRead(" "));
    Serial.println(state_mode);
    if(state_mode==FADE){
      if(LAST_STATE!=PASSIVE){
        STATE=FADE;
        return;
      }
      STATE=PASSIVE;
      return;
    } 
    PID_vel->reset();

    if(state_mode==(STAND_BY)){
      STATE=STAND_BY;
    }else if (state_mode==(ACTIVE_PLUS)){
      STATE=ACTIVE_PLUS;
    }
  }else if(modo==AUTO){
    odometry_calc();
    if(duration-clock->current_min()<=sample_ex){
    STATE=STAND_BY;
    health_p+=delta_cicles;
    delta_cicles=0;
    Serial.println("health_p");
    Serial.println(health_p);
    if(health_p<LIMITE_DEBILIDADO){
      HEALTH_STATE=WEEK;
      return;
    }else if(health_p<LIMITE_SAUDAVEL){
      HEALTH_STATE=NORMAL;
      return;
    }else{
      HEALTH_STATE=STRONG; 
      return;
    }
    }else{
      switch (HEALTH_STATE)
      {
      case WEEK:
        if(LAST_STATE!=PASSIVE){
          STATE=FADE;
          return;
        }
        STATE=PASSIVE;
        return;
      case NORMAL:
        if(actual_rpm<goal_vel){
          if(LAST_STATE!=PASSIVE){
            STATE=FADE;
            return;
          }
          STATE=PASSIVE;
          return;
        }
        PID_vel->reset();
        STATE=STAND_BY;
        return;
      case STRONG:
        STATE=ACTIVE_PLUS;
        return;
      }
    }
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
      Serial.println("delta_cicles");
      Serial.println(delta_cicles);
      // avoid overflowing variable pulses on encoder lib
      if (current_position>10000 || current_position<-10000){
        encoder->setPulses(0);
        last_position=0;
        last_cicles=0;
      }
    }
    return;
}