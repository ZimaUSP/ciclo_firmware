
/*
#include<LiquidCrystal_I2C.h>
#include<SimpleTimer.h>
#include<Encoder.hpp>
#include<H_bridge_controller.hpp>

#include<normal.hpp>


    
normal::normal(int contador,int *tempo,double *lista_values,LiquidCrystal_I2C lcd, SimpleTimer rpmTime,SimpleTimer lcd_timer, Encoder *encoder, H_bridge_controller *motor){
    this->contador = contador;
    this->lcd = lcd;
    this->encoder =  encoder;
    this->rpmTime = rpmTime;
    this->motor = motor;
    this->encoder = encoder;
    this->tempo = tempo;
}


void normal::executaNormal(){
    double delta_pulses,current_pulses;
    double actual_rpm;
    double last_pulses = encoder->getPulses();
    contador = 0;
    lcd_timer.reset();
    rpmTime.reset();

    while (!lcdTimer.IsRead()) {
        if (rpmTime.getTimePassed() > 400) {
          current_pulses = encoder->getPulses();
          delta_pulses = current_pulses - last_pulses;
          actual_rpm = delta_pulses * 1.01;
    
          if(contador < MAX_SAMPLES) {
            lista_values[contador] = actual_rpm;
            tempo[contador]=contador*sample_t;
            contador++;
          }
          Serial.print(actual_rpm);
          Serial.print(", ");
          Serial.println(display.lcd_timer.getTimePassed());
    
          //resetEncoderIfExceedsLimit();
          resetaEncoder(current_pulses,last_pulses);
          rpmTime.reset();
          last_pulses = current_pulses;
          Serial.print("; actual rpm: ");
          Serial.print(actual_rpm);
          Serial.print("; time: ");
          Serial.println(lcd_timer.getTimePassed());
    
        }
        
    lcd.setCursor(0, 1);
    lcd.print("Frequency: ");
    lcd.print(actual_rpm);
    printTime();
  }
    saved->push_normal(tempo, lista_values, MAX_SAMPLES);
}

void normal::resetaEncoder(double current_pulses,double last_pulses){
  if (current_pulses > MAX_ENCODER_VALUE || current_pulses < -MAX_ENCODER_VALUE) {
    encoder->setPulses(0);
    last_pulses = 0;
  }
}


/*

void normal() {
  contador = 0;
  lcd_timer.reset();
  rpmTime.reset();
  //website_data();
  while (!lcd_timer.isReady()) {  //tempo nao acaba

    if (rpmTime.getTimePassed() > 400) {
      current_pulses = encoder->getPulses();
      delta_pulses = current_pulses - last_pulses;
      double revolutions = delta_pulses/pulses_per_rev;
      actual_rpm = revolutions*(60000/400);

      if(contador < MAX_SAMPLES) {
        lista_values[contador] = torque;
        tempo[contador]=contador*sample_t;
        contador++;
      }
      
      resetEncoderIfExceedsLimit();
      rpmTime.reset();
      last_pulses = current_pulses;
      Serial.print("; actual rpm: ");
      Serial.print(actual_rpm);
      Serial.print("; time: ");
      Serial.println(lcd_timer.getTimePassed());

    }
    lcd.setCursor(0, 1);
    lcd.print("Frequency: ");
    lcd.print(actual_rpm);
    printTime();
  }
  saved->push_normal(tempo, lista_values, MAX_SAMPLES);
}




*/


#include "normal.hpp"

#define MAX_SAMPLES 1000
#define MAX_ENCODER_VALUE 10000

normal::normal(int contador, int *tempo, double *lista_values, LiquidCrystal_I2C &lcd,
               SimpleTimer &rpmTime, SimpleTimer &lcd_timer, Encoder *encoder,
               H_bridge_controller *motor, Memory *saved, int sample_time) :
    lcd(lcd), rpmTime(rpmTime), lcd_timer(lcd_timer)
{
    this->contador = contador;
    this->tempo = tempo;
    this->lista_values = lista_values;
    this->encoder = encoder;
    this->motor = motor;
    this->saved = saved;
    this->sample_time = sample_time;
}

void normal::printTime() {
    lcd.setCursor(0, 0);
    lcd.print("Duration: ");
    char t[10];
    sprintf(t, "%02d", lcd_timer.getMinutes());
    lcd.print(t);
    lcd.print(":");
    sprintf(t, "%02d", lcd_timer.getSeconds());
    lcd.print(t);
}

void normal::executaNormal(){
    double delta_pulses, current_pulses;
    double actual_rpm;
    double last_pulses = encoder->getPulses();
    contador = 0;
    lcd_timer.reset();
    rpmTime.reset();

    while (!lcd_timer.isReady()) {
        if (rpmTime.getTimePassed() > 400) {
            current_pulses = encoder->getPulses();
            delta_pulses = current_pulses - last_pulses;
            actual_rpm = delta_pulses * 1.01;
    
            if(contador < MAX_SAMPLES) {
                lista_values[contador] = actual_rpm;
                tempo[contador] = contador * sample_t;
                contador++;
            }
            
            Serial.print(actual_rpm);
            Serial.print(", ");
            Serial.println(lcd_timer.getTimePassed());
    
            resetaEncoder(current_pulses, last_pulses);
            rpmTime.reset();
            last_pulses = current_pulses;
            Serial.print("; actual rpm: ");
            Serial.print(actual_rpm);
            Serial.print("; time: ");
            Serial.println(lcd_timer.getTimePassed());
        }
        
        lcd.setCursor(0, 1);
        lcd.print("Frequency: ");
        lcd.print(actual_rpm);
        printTime();
    }
    saved->push_normal(tempo, lista_values, MAX_SAMPLES);
}

void normal::resetaEncoder(double current_pulses, double last_pulses){
    if (current_pulses > MAX_ENCODER_VALUE || current_pulses < -MAX_ENCODER_VALUE) {
        encoder->setPulses(0);
        last_pulses = 0;
    }
}