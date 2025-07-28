#include "Passive.hpp"

Passive::Passive(SimpleTimer lcd_timer, SimpleTimer rpmTimer, Encoder* encoder, Memory* saved, PID* PID_vel, H_bridge_controller* motorController, LiquidCrystal_I2C* lcd){
  this->lcd_timer = lcd_timer;
  this->rpmTime = rpmTimer;
  this->encoder = encoder;
  this->saved = saved;
  this->PID_vel = PID_vel;
  this->motorController = motorController;
  this->lcd = lcd;
};

void Passive::reset_mode(){
  contador = 0;
  lcd_timer.reset();
  rpmTime.reset();
  for (int i = 0; i < MAX_SAMPLES; i++) {
    lista_values[i] = 3.0;
    tempo[i]=3;
  }
}

void Passive::resetEncoderIfExceedsLimit() {
  if (current_pulses > MAX_ENCODER_VALUE || current_pulses < -MAX_ENCODER_VALUE) {
    encoder->setPulses(0);
    last_pulses = 0;
  }
}

void Passive::printTime() {
  lcd->setCursor(0, 0);
  lcd->print("Duration: ");
  sprintf(t, "%02d", lcd_timer.getMinutes());
  lcd->print(t);
  lcd->print(":");
  sprintf(t, "%02d", lcd_timer.getSeconds());
  lcd->print(t);
}

void Passive::controlMotorSpeedWithPID() {
  output = PID_vel->computePID(actual_rpm, goal_rpm, tolerance);
  
  Serial.print("goal: ");
  Serial.print(goal_rpm);
  Serial.print("; actual rpm: ");
  Serial.print(actual_rpm);
  Serial.print("; tolerancia: ");
  Serial.print(tolerance);
  Serial.print("; output: ");
  Serial.println(output);
  
  if (output < 0) {
    Serial.println("going left");
    output = max(output, -MAX_PWM);
    motorController->Set_L(-output);
  } else {
    Serial.println("going right");
    output = min(output, MAX_PWM);
    motorController->Set_R(output);
  }
}

void Passive::new_torques(){
  while (!lcd_timer.isReady()) {
  if (rpmTime.getTimePassed() > sample_t) {
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
    Serial.println(lcd_timer.getTimePassed());

    resetEncoderIfExceedsLimit();
    controlMotorSpeedWithPID();

    rpmTime.reset();
    last_pulses = current_pulses;
  }
    
  printTime();
  }
  saved->push_passivo(tempo, lista_values, MAX_SAMPLES);
}

void Passive::init_passive(Passive passive){
  passive.reset_mode();
  passive.new_torques();
}