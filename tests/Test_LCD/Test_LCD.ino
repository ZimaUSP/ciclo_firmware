// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Timer.hpp"
#include "Button.hpp"

int pageSelec = 0;

// State modes  
char STATE = MODE;

// Declair constructors

LiquidCrystal_I2C lcd(0x27,16,2); // LCD lib

Button *btn;

 
// Program variable



unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;
bool joystick_check;
unsigned int joystick_state;


void setup(){
  Serial.begin (9600);



  btn = new Button(btn_pin,2);
  btn->init();  

  lcd.init();                      // initialize the lcd 
  last_t=millis();
  STATE = STAND_BY;  
 
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

void print_mode(){
    lcd.setCursor(0,0);
    
    if (STATE == NORMAL)
    {
        lcd.print("Modo escolhido:");
        lcd.setCursor(0,1);
        lcd.print("Normal         ");
        lcd.setCursor(0,1);

    }else if (STATE == PASSIVE)
    {
        lcd.print("Modo escolhido:");
        lcd.setCursor(0,1);
        lcd.print("Passive        ");
        lcd.setCursor(0,1);

    }else if(STATE==FADE){
        lcd.print("Modo escolhido:");
        lcd.setCursor(0,1);
        lcd.print("Fade         ");
        lcd.setCursor(0,1);
    }
}

void loop(){
    if (STATE == STAND_BY )
    {
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
            if(pageSelec != 3){
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

    }        
}

  /*
  while(!btn->getPress()){
    lcd.setCursor(0,2);
    lcd.print("Tempo de duracao");
    lcd.setCursor(0,2);
    lcd.print("               ");

    lcd.setCursor(0,3);
  }
*/
