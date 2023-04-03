// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"
#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "PID.hpp"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Timer.hpp"
#include "Button.hpp"

// State modes  
char STATE = MODE;

// Declair constructors
Encoder *encoder;
H_bridge_controller *Motor;
PID *PID_vel;
LiquidCrystal_I2C lcd(0x27,16,2); // LCD lib
Timer *clock;
Button *btn;
int *time_min;
int *time_seg;
clock = 
// Program variable
int output;


unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;

double current_position;
double last_position;
double delta_position;

setup(){
Serial.begin (9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,Mode);
  encoder->init();

  Motor= new H_bridge_controller( r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  Motor->init();

  PID_vel = new PID(kp,ki,kd,i_saturation);
  btn = new Button(btn_pin,2);
  btn->init();  

  clock= new Timer(); 
  clock->init();

  lcd.init();                      // initialize the lcd 
  last_t=millis();
  
  last_position=encoder->getPosition();
 
}
loop(){
print_LCD();


}

void print_LCD(){
    lcd.backlight();
    lcd.setCursor(0,0);
    
    if (STATE == STAND_BY)
    {
        lcd.print("Modo: Normal   ");
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);

    }else if (STATE == PASSIVE)
    {
        lcd.print("Modo: Passive");
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);   
    }else if(STATE==MODE){
        lcd.print("Escolha o modo ");
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
    if(STATE==AUTO){
        lcd.print("  Automatico   ");
    }else if(STATE==MANUAL){
        lcd.print("    Manual    ");
    }
    }

    }

void select_function(){
  int pageSelec = map(analogRead(pot_pin),0,4095,0,5);
  
  while (!btn->getPress()){
    if(pageSelec == 0){
        lcd.print("Escolha o modo ");
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
    }
    else if(pageSelec == 1){
        lcd.print("Modo: Normal   ");
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
        STATE = STAND_BY;
    }
    else if(pageSelec == 2){
        lcd.print("Modo: Passive");
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
        STATE = PASSIVE;
    }
    else if(pageSelec == 3){
        lcd.print("Modo: Manual");
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
        STATE = MANUAL;
    }
  }
  
  while(!btn->getPress()){
    lcd.setCursor(0,2);
    lcd.print("Tempo de duracao");
    lcd.setCursor(0,2);
    lcd.print("               ");
    *time_min = clock->current_min();
    *time_seg = clock->current_sec();
    lcd.setCursor(0,3);
    lcd.print("Time: %d:%d", *time_min, *time_seg);
  }
}
