#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "Button.hpp"
#include "config.hpp"
Button *btn;
Encoder *encoder;
H_bridge_controller *BTS;

unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;

double current_pulses;
double last_pulses;
double delta_pulses;

double delta_ciclos;

int output;
float goal_vel;
float goal_rpm;

float actual_vel;
float actual_rpm;
int a,b,c,d,e;
int count = 0;
 
void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,125,Mode); // modificar lib do encoder e retirar posicao
  encoder->init();

  BTS = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  BTS->init();

  btn= new Button(btn_pin,0);
  btn->init();


  last_t=millis(); //t0
  last_pulses=encoder->getPulses(); //p0
  }
   
void loop() {
    current_t=millis(); //t1
    delta_t = current_t-last_t; // t1-t0
    while (btn->getPress()||analogRead(pot_pin)<100)
    {
      BTS->Set_L(0);
    }
    
    if(delta_t>sample_t){
      current_pulses = encoder->getPulses(); //t1
      delta_pulses = current_pulses - last_pulses;//t1-t0
      Serial.print("delta pulses: ");
      Serial.println(delta_pulses);

      delta_ciclos = delta_pulses/pulses_per_rev;
      Serial.print("delta ciclos: ");
      Serial.println(delta_ciclos);

      actual_rpm = delta_ciclos*60000/sample_t;

      last_t=current_t;
      last_pulses=current_pulses;

      Serial.print("rpm: ");
      Serial.println(actual_rpm);

      
      if (current_pulses>10000 || current_pulses<-10000){ // zerar encoder para não 
        encoder->setPulses(0);
        last_pulses=0;

      }
      
    }
    
    BTS->Set_R(map(analogRead(pot_pin),0,4095,0,255));
   

      /*Serial.print("pwm: ");
      Serial.println(map(analogRead(pot_pin),0,4095,0,255));*/
      
}