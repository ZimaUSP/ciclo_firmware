#include "Passive.hpp"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Control
  double last_pulses;
  //Constructor
  LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD lib
  SimpleTimer lcd_timer;
  SimpleTimer rpmTime;
  //Objects
  Encoder* encoder;
  H_bridge_controller* motorController;
  PID* PID_vel;
  Memory* saved;

  //Inicialização dos objetos
  encoder = new Encoder(a_pin, b_pin, 0, Nominal_pulses, Mode);
  encoder->init();

  motorController = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  motorController->init();

  PID_vel = new PID(1.4, 0.008, kd, i_saturation);
  lcd.init();

  //Inicialização das variáveis
  last_pulses = encoder->getPulses();
  saved = new Memory();

  //Classe Passive

  lcd_timer.setInterval(1 * 60000);
  Passive* passive = new Passive(lcd_timer, rpmTime, encoder, saved, PID_vel, motorController, &lcd);
  Serial.println("init...");
  passive->init_passive();
}

void loop() {
  // put your main code here, to run repeatedly:

}
