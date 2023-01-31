#include "H_bridge_controller.hpp"
#include "config.hpp"
H_bridge_controller *Motor;
double acs,med,avg_acs;

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
  
  

}
void loop() {
  pot = analogRead(pot_pin);
  pwm = map(pot,0,4095,0,255);
  acs = map(analogRead(acs_pin),0,4095,0,330);
  //Serial.print("cru:");
  //Serial.println(acs);
  acs = -0.0443*acs*acs*acs/1000000+0.1494*acs*acs/10000+0.9748*acs/100+0.0674-0.07;   //valor lido em V de 0 até 3.3 calibrado
  Motor->Set_R(pwm);
  //Serial.print("Voltagem:");
  //Serial.println((acs/100));
  //Serial.print("corrente:");
  //Serial.println(14.6*(acs)-24); 
  //Serial.print("acs:");
  //Serial.println(analogRead(acs_pin)); 
  //Serial.print("acs map:");
  //Serial.print("calibrado:");
  //Serial.println(acs);

  for(int i=0; i<1000;i++){
    acs = map(analogRead(acs_pin),0,4095,0,330);
    avg_acs += -0.0443*acs*acs*acs/1000000+0.1494*acs*acs/10000+0.9748*acs/100+0.0674-0.07;
  }
  avg_acs = avg_acs/1000;
  //Serial.println(avg_acs);
  Serial.print("corrente:");
  Serial.println(14.6*(avg_acs)-24); 
  
  delay(10);
}

 
    
  


