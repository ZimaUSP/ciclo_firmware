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
    switch(STATE) {
        case STAND_BY :
        {
        while (!btn->getPress()){
            pageSelec = map(analogRead(pot_pin),0,4095,0,2);
            select_function();
            lcd.noBacklight();
        }
        print_mode();
        delay(1000);
        t_Duration = duration();
        lcd.clear();
        LCD_timer.setInterval(t_Duration*60000);
        LCD_timer.reset();
        }
    }
    
    lcd.setCursor(0,0);
    lcd.print("Duration: ");
    Serial.print("minutos: ");
    Serial.println(LCD_timer.getMinutes());
    sprintf(t,"%02d",LCD_timer.getMinutes());
    lcd.print(t);
    lcd.print(":");
    Serial.print("segundos: ");

    Serial.println(LCD_timer.getSeconds());

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
    
    lcd.setCursor(0,0);
    lcd.print("              ");
    while (!btn->getPress()){
        t_Duration = map(analogRead(pot_pin),0,4095,0,59);        
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
