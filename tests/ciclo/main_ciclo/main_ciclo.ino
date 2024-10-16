#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "PID.hpp"
#include "config.hpp"
#include "Button.hpp"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>



//******CONSTRUCTOR******//

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD lib
SimpleTimer lcd_timer;
SimpleTimer rpmTime;


//******OBJECTS******//
Encoder* encoder;
H_bridge_controller* motorController;
Button* btn;
PID* PID_vel;

//******GLOBAL VARIABELS******//

// Time
unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;
int t_Duration;
char t[2];

// Control
double current_pulses;
double last_pulses;
double delta_pulses;
double delta_ciclos;
float actual_rpm;
int goal_rpm;
int verif;
int output;
char STATE = MODE;
double pageSelec;
double revolutions;
bool joystick_check;


//******FUNCTIONS******//

// Get frequecy of pot (incluir na classe do potenciometro)
int mapPotValueToRPM(int potValue) {
  return map(potValue, 0, 4095, 0, 50);  // rpm
}

// Control to evit Overflow
void resetEncoderIfExceedsLimit() {
  if (current_pulses > MAX_ENCODER_VALUE || current_pulses < -MAX_ENCODER_VALUE) {
    encoder->setPulses(0);
    last_pulses = 0;
  }
}

// PID control
void controlMotorSpeedWithPID() {
  output = PID_vel->computePID(actual_rpm, goal_rpm, tolerance);

  if (output < 0) {
    output = max(output, -MAX_PWM);
    motorController->Set_L(-output);
  } else {
    output = min(output, MAX_PWM);
    motorController->Set_R(output);
  }
}

// Turns it off and on to reset the program

void reset() {
  ESP.restart();
}
// Implemetation passive Mode (Criar Classe passivo para implementar esses controle e evitar de ter muita coisa na main)
void passivo() {
  lcd_timer.reset();
  rpmTime.reset();
  while (!lcd_timer.isReady()) {  //tempo nao acaba



    if (rpmTime.getTimePassed() > sample_t) {
      current_pulses = encoder->getPulses();
      delta_pulses = current_pulses - last_pulses;
      actual_rpm = delta_pulses * 1.01;

      Serial.print(actual_rpm);
      Serial.print(", ");
      Serial.println(lcd_timer.getTimePassed());


      resetEncoderIfExceedsLimit();
      controlMotorSpeedWithPID();

      rpmTime.reset();
      last_pulses = current_pulses;
    }
    printTime();
  }
}

// Implemetation normal Mode (Criar Classe normal para implementar esses controle e evitar de ter muita coisa na main)

void normal() {
  lcd_timer.reset();
  rpmTime.reset();
  while (!lcd_timer.isReady()) {  //tempo nao acaba

    if (rpmTime.getTimePassed() > 400) {
      current_pulses = encoder->getPulses();
      delta_pulses = current_pulses - last_pulses;
      actual_rpm = delta_pulses / 400;
      resetEncoderIfExceedsLimit();
      rpmTime.reset();
      last_pulses = current_pulses;
    }
    lcd.setCursor(0, 1);
    lcd.print("Frequency: ");
    sprintf(t, "%02d", actual_rpm);
    lcd.print(t);
    printTime();
  }
}

// Select funcition
void selectFunction() {
  if (pageSelec ==0 ) {
    lcd.print("Escolha o modo ");
    lcd.setCursor(0, 1);
    lcd.print("Modo: Normal   ");
    lcd.setCursor(0, 0);
    STATE = NORMAL;
  } else if (pageSelec == 1) {
    lcd.print("Escolha o modo ");
    lcd.setCursor(0, 1);
    lcd.print("Modo: Passive   ");
    lcd.setCursor(0, 0);
    STATE = PASSIVE;
  }
  
}

// Print in LCD the MODE select
void printMode() {
  lcd.setCursor(0, 0);
  switch (STATE) {
    case NORMAL:
      lcd.print("Selecionado:  ");
      lcd.setCursor(0, 1);
      lcd.print("Modo Normal   ");
      lcd.setCursor(0, 1);
      return;

    case PASSIVE:
      lcd.print("Selecionado:  ");
      lcd.setCursor(0, 1);
      lcd.print("Modo Passivo  ");
      lcd.setCursor(0, 1);
      return;
    case FADE:
      lcd.print("Selecionado:  ");
      lcd.setCursor(0, 1);
      lcd.print("Modo Fade     ");
      lcd.setCursor(0, 1);
      return;
  }
}


// Select Duration
int duration() {
  int t_Duration;
  lcd.setCursor(0, 0);
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
    lcd.setCursor(0, 0);  // MAX(15,1) linha, coluna
    lcd.print("Duration: ");
    sprintf(t, "%02d", t_Duration);
    lcd.print(t);
    lcd.print(":00  ");
  }
  return t_Duration;
}

// Print stopwatch in LCD

void printTime() {
  lcd.setCursor(0, 0);
  lcd.print("Duration: ");
  sprintf(t, "%02d", lcd_timer.getMinutes());
  lcd.print(t);
  lcd.print(":");
  sprintf(t, "%02d", lcd_timer.getSeconds());
  lcd.print(t);
}

void printFrequency() {
  lcd.setCursor(0, 1);
  lcd.print("Frequency: ");
  sprintf(t, "%02d", goal_rpm);
  lcd.print(t);
}


// Get frequency (Implementar como Vetor quando criar a Classe)
int goalRPM() {

   while (!btn->getPress()){
         if (analogRead(pot_pin) != 0 && analogRead(pot_pin) != 4095)
            {
                joystick_check=true;
            }
            if (analogRead(pot_pin) == 0 && joystick_check)
            {
                if(goal_rpm !=0 ){
                    goal_rpm--;
                    joystick_check = false;
                }
            }
            else if (analogRead(pot_pin) == 4095 && joystick_check)
            {
                if(goal_rpm != 50){
                    goal_rpm++;
                    joystick_check = false;
                }
            }   
    printFrequency();
   }
  return goal_rpm;
}


// Verification

int verification() {
  lcd.clear();
  while (!btn->getPress()) {
    if (analogRead(pot_pin)==0)
    {
      verif = 0;
    }
    if (analogRead(pot_pin)==4095)
    {
      verif = 1;
    }
    lcd.setCursor(0, 0);
    lcd.print("Freq: ");
    sprintf(t, "%02d", goal_rpm);
    lcd.print(t);
    lcd.print("|");
    lcd.print("Time:");
    sprintf(t, "%02d", lcd_timer.getInterval() / 60000);
    lcd.print(t);
    lcd.setCursor(0, 1);
    if (verif == 0) {
      lcd.print("Nao            ");
      lcd.setCursor(0, 1);
      lcd.noBacklight();
    }
    if (verif == 1) {
      lcd.print("Sim            ");
      lcd.setCursor(0, 1);
      lcd.noBacklight();
    }
  }
  lcd.clear();
  return verif;
}



//******MAIN******//

void setup() {
  Serial.begin(9600);

  // Inicialização dos objetos
  encoder = new Encoder(a_pin, b_pin, 0, Nominal_pulses, Mode);
  encoder->init();

  motorController = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  motorController->init();

  PID_vel = new PID(1.4, 0.008, kd, i_saturation);


  // Inicialização das variáveis
  last_t = millis();
  last_pulses = encoder->getPulses();

  btn = new Button(btn_pin, 2);
  btn->init();

  lcd.init();  // initialize the lcd
  last_t = millis();


  STATE = STAND_BY;
}

void loop() {

  switch (STATE) {
    case PASSIVE:
      goal_rpm = goalRPM();
      delay(500);
      lcd_timer.setInterval(duration() * 60000);
      delay(500);
      lcd.clear();
      verif = verification();
      delay(500);
      if (verif >= 1) {
        delay(500);
      }
      if (verif < 1) {
        STATE = STAND_BY;
        delay(500);
        return;
      }
      passivo();
      motorController->Set_L(0);
      STATE = STAND_BY;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FIM");
      delay(2000);
      reset();
      return;

    case STAND_BY:
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
            if(pageSelec != 1){
                pageSelec++;
                joystick_check = false;
            }
        }
        
        selectFunction();
        lcd.noBacklight();
      }
      printMode();
      delay(500);
      return;

    case NORMAL:
      delay(100);
      motorController->Set_L(0);
      lcd_timer.setInterval(duration() * 60000);
      delay(500);
      lcd.clear();

      normal();

      delay(500);
      reset();
      return;
  }
}