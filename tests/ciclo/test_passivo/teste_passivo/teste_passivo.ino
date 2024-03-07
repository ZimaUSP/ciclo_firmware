#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "PID.hpp"
#include "config.hpp"
#include "Button.hpp"

// Constantes
const int MAX_ENCODER_VALUE = 10000;

// Objetos
Encoder* encoder;
H_bridge_controller* motorController;
Button* btn;
PID* PID_vel;

// Variáveis globais
unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;

double current_pulses;
double last_pulses;
double delta_pulses;
double delta_ciclos;

int output;
float goal_vel;
long goal_rpm;
float actual_vel;
float actual_rpm;
int count = 0;

// Funções
float mapPotValueToRPM(int potValue);
void resetEncoderIfExceedsLimit();
void controlMotorSpeedWithPID();

void setup() {
  Serial.begin(9600);

  // Inicialização dos objetos
  encoder = new Encoder(a_pin, b_pin, 0, Nominal_pulses, Mode);
  encoder->init();

  motorController = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  motorController->init();

  PID_vel = new PID(1.4, 0.008, kd, i_saturation);
 
  btn = new Button(btn_pin, 2);

  // Inicialização das variáveis
  last_t = millis();
  last_pulses = encoder->getPulses();
}

void loop() {
  current_t = millis();
  delta_t = current_t - last_t;

  // Parar o motor se o potenciômetro estiver abaixo do valor mínimo
  while (analogRead(pot_pin) < 100) {
    motorController->Set_L(0);
  }

  if (delta_t > sample_t) {
    current_pulses = encoder->getPulses();
    delta_pulses = current_pulses - last_pulses;
    actual_rpm = delta_pulses *1.01;

    resetEncoderIfExceedsLimit();
    
    goal_rpm = mapPotValueToRPM(analogRead(pot_pin));

    controlMotorSpeedWithPID();

    last_t = current_t;
    last_pulses = current_pulses;
  }
}

float mapPotValueToRPM(int potValue) {
  return map(potValue, 0, 4095, 0, 50); // rpm
}

void resetEncoderIfExceedsLimit() {
  if (current_pulses > MAX_ENCODER_VALUE || current_pulses < -MAX_ENCODER_VALUE) {
    Serial.println("Encoder reset");
    encoder->setPulses(0);
    last_pulses = 0;
  }
}

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
