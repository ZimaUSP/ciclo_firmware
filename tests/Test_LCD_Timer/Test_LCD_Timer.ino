// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Timer.hpp"
#include "Button.hpp"
#include <SimpleTimer.h>

int pageSelec;
int t_Duration;
char t[2];
bool joystick_check;

// State modes  
char STATE = MODE;

// Declair constructors

LiquidCrystal_I2C lcd(0x27,16,2); // LCD lib

Button *btn;

SimpleTimer LCD_timer;

// Program variable

unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;


void setup(){
  Serial.begin (9600);

  btn = new Button(btn_pin,2);
  btn->init();  
  lcd.init();                      // initialize the lcd 
  last_t=millis();
  STATE = STAND_BY;
}

void loop(){
    while (LCD_timer.isReady()){
        lcd.clear();
        while (!btn->getPress()){
            if (analogRead(pot_pin) != 0 && analogRead(pot_pin) != 4095)
            {
                joystick_check=true;
            }
            if (analogRead(pot_pin) == 0 && joystick_check)
            {
                if(pageSelec !=0 ){
                    pageSelec--;
                    joystick_check = false;
                }

            }
            else if (analogRead(pot_pin) == 4095 && joystick_check)
            {
                if(pageSelec != 2){
                    pageSelec++;
                    joystick_check = false;
                }
            }
        
        select_function();
        lcd.noBacklight();
        Serial.println(analogRead(pot_pin));
        Serial.println(joystick_check);
        }
        print_mode();
        delay(1000);
        t_Duration = duration();
        lcd.clear();
        LCD_timer.setInterval(t_Duration*60000);
        LCD_timer.reset();
        
    }
    
    lcd.setCursor(0,0);
    lcd.print("Duration: ");
  
    sprintf(t,"%02d",LCD_timer.getMinutes());
    lcd.print(t);
    lcd.print(":");
   
    sprintf(t,"%02d",LCD_timer.getSeconds());
    lcd.print(t);
    
}


void print_mode(){
    lcd.setCursor(0,0);
    switch(STATE) {    
        case NORMAL:
    
        lcd.print("Selecionado:  ");
        lcd.setCursor(0,1);
        lcd.print("Modo Normal   ");
        lcd.setCursor(0,1);
        return;

        case PASSIVE:
        lcd.print("Selecionado:  ");
        lcd.setCursor(0,1);
        lcd.print("Modo Passivo  ");
        lcd.setCursor(0,1);
        return;

        case FADE: 
        lcd.print("Selecionado:  ");
        lcd.setCursor(0,1);
        lcd.print("Modo Fade     ");
        lcd.setCursor(0,1);
        return;
    }
    }

void select_function(){
    
    if(pageSelec == 0){
        lcd.print("Escolha o modo ");
        lcd.setCursor(0,1);
        lcd.print("Modo: Normal   ");   
        lcd.setCursor(0,0);
        STATE = NORMAL;
    }
    else if(pageSelec == 1){
        lcd.print("Escolha o modo ");
        lcd.setCursor(0,1);
        lcd.print("Modo: Passive   ");
        lcd.setCursor(0,0);
        STATE = PASSIVE;
    }
    else if(pageSelec == 2){
        lcd.print("Escolha o modo ");
        lcd.setCursor(0,1);
        lcd.print("Modo: Fade   ");
        lcd.setCursor(0,0);
        STATE = FADE;
    }
  }

  int duration(){
    int t_Duration;
    
    t_Duration=0;
    lcd.setCursor(0,0);
    lcd.print("              ");
    while (!btn->getPress()){
         if (analogRead(pot_pin) != 0 && analogRead(pot_pin) != 4095)
            {
                joystick_check=true;
            }
            if (analogRead(pot_pin) == 0 && joystick_check)
            {
                if(t_Duration !=0 ){
                    t_Duration--;
                    joystick_check = false;
                }
            }
            else if (analogRead(pot_pin) == 4095 && joystick_check)
            {
                if(t_Duration != 10){
                    t_Duration++;
                    joystick_check = false;
                }
            }        
        lcd.setCursor(0,0); // MAX(15,1) linha, coluna
        lcd.print("Duration: ");
        sprintf(t,"%02d",t_Duration);
        lcd.print(t);
        lcd.print(":00  ");  
    }
    return t_Duration;


  }
  /*
  sprintf(t,"%02d",clock->current_min());
    lcd.print(t);
    lcd.print(":");
    sprintf(t,"%02d",clock->current_sec());
    lcd.print(t);
*/
