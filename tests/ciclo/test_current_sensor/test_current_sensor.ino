#include "H_bridge_controller.hpp"
#include "config.hpp"
#include "current_sensor.hpp"
H_bridge_controller *Motor;
current_sensor *cur;
double acs,acs_teste,acs_med;
bool joystick_check;

int offset, pot, pwm,sum,i;

void setup() {
  Serial.begin(9600);
  Motor= new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  Motor->init();
  Motor->Set_R(0);
  
  for(int i=0; i<100;i++){
    offset += map(analogRead(acs_pin),0,4095,0,330);
    delay(10);
  }
  offset = offset/10000;
  cur = new current_sensor(acs_pin, 1850, 20);
  acs=0;
  pwm = 40;
  acs_med = 0;
  acs_teste = 0;
}

void loop(){
  Motor->Set_L(pwm);
  acs = cur->get_current();
  Serial.print("        /       ");
  Serial.print("Corrente: ");
  Serial.print(acs);
  Serial.println("A");
  Serial.println("-----------");
  delay(500);
} 
    

  


