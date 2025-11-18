// TESTE DO MODO RESISTIVO(CLASSE)

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>
#include "soc/sens_reg.h" // ESP32

// Inclui  classe e as configurações
#include "Resistivo.hpp"
#include "config.hpp"


#ifndef MAX_SAMPLES
  #define MAX_SAMPLES 5
#endif

// Garante que o pot_pin esteja definido (veio do config.hpp)
#ifndef pot_pin
  #define pot_pin 34 
#endif

void setup() {
  Serial.begin(9600);
  while(!Serial) delay(10);

  Serial.println("--- INICIANDO SETUP DE TESTE ---");

  // Stack
  LiquidCrystal_I2C lcd(0x27, 16, 2); 
  SimpleTimer lcd_timer;
  SimpleTimer rpmTime;
  SimpleTimer torque_Time; 

  // Ponteiros
  Encoder* encoder;
  H_bridge_controller* motorController;
  PID* PID_vel;
  Memory* saved;
  
  // Novos objetos
  Button* btn;
  Joystick* joy;
  current_sensor* cur;

  // Inicialização do Hardware Básico
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Setup Init...");

  // Instanciação de objetos
  
  encoder = new Encoder(a_pin, b_pin, 0, Nominal_pulses, Mode);
  encoder->init();

  motorController = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  motorController->init();
  
  // Ponte H
  #ifdef enable_pin
    pinMode(enable_pin, OUTPUT);
    digitalWrite(enable_pin, HIGH); 
  #endif

  PID_vel = new PID(kp, ki, kd, i_saturation);

  saved = new Memory();
  btn = new Button(btn_pin);
  cur = new current_sensor(acs_pin);

  
  uint32_t reg_val = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);

  joy = new Joystick(pot_pin, reg_val, reg_val);

  Serial.println("Hardware Configurado.");

  // Classe Resistivo
  resistivo* myResistivo = new resistivo(
      &lcd, 
      &lcd_timer, 
      &rpmTime, 
      &torque_Time, 
      encoder, 
      saved, 
      PID_vel, 
      motorController, 
      btn, 
      joy, 
      cur
  );

  lcd.clear();
  lcd.print("Resistivo Ready");
  delay(1000);
  
  Serial.println("Iniciando Loop da Classe...");
  
  // Execução
  myResistivo->executarLogicaResistivo();
}

void loop() {
  
}