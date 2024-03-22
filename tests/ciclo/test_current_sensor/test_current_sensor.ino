#include "H_bridge_controller.hpp"
#include "config.hpp"
H_bridge_controller *Motor;
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
  acs=0;
  pwm = 40;
  acs_med = 0;
  acs_teste = 0;
}

void loop() {
  //Serial.println(pwm);
  //pot = analogRead(pot_pin);
  //pwm = map(pot,0,4095,0,200); //50 100 130 160 190
  /*
      if (analogRead(pot_pin) != 0 && analogRead(pot_pin) != 4095)
        {
          joystick_check=true;
        }
        
      if (analogRead(pot_pin) == 0 && joystick_check && pwm >= 5)
      {
          pwm = pwm - 5;
          Serial.println(pwm);
      }
      else if (analogRead(pot_pin) == 4095 && joystick_check && pwm <= 200)
      {
          pwm = pwm + 5;
          Serial.println(pwm);
      }*/
  Motor->Set_R(pwm);
  
  //Serial.print("cru:");
  //Serial.println(acs);
  //acs = -0.0443*acs*acs*acs/1000000+0.1494*acs*acs/10000+0.9748*acs/100+0.0674-0.07;   //valor lido em V de 0 até 3.3 calibrado
  
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
      acs +=((-5.59)*(analogRead(acs_pin)/1000)+13.3)*(0.644)+2.234-2.5;//map(analogRead(acs_pin),2450,0,2200,800);   
     
  }
  acs_med = acs/1000;
//  Serial.print("Leitura media:");
//  Serial.println(acs_med);
  acs = acs/100;
 /* for(int i=0; i<100;i){
    acs_teste =((-5.59)*(analogRead(acs_pin)/1000)+13.3)*(0.644)+2.234-3; //map(analogRead(acs_pin),2450,0,2200,0)-170;
    if (acs_teste-acs_med<100 || acs_teste-acs_med >-100){
      acs+= acs_teste;
      i++;
  //    Serial.print("Somou:");
  //    Serial.println(acs_teste);
    }    
  }
  */
  Serial.print("Leitura final: ");
  Serial.println(acs/10); 
 // Serial.print(",");
 // Serial.print(pwm);
 // Serial.println(")");
  
  acs =0;/*
  Serial.print("Voltagem saida sensor: ");
  Serial.println((analogRead(acs_pin)); 
  Serial.print("PWM: ");
  Serial.println(pwm);*/
  
  //Serial.println(avg_acs);
  
  
}

 
    
  


