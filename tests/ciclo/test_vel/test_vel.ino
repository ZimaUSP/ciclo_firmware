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

double current_position;
double last_position;
double delta_position;

int output;
float goal_vel;
float goal_rpm;

float actual_vel;
float actual_rpm;
int a,b,c,d,e;
int count = 0;
 
void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,perimeter_pulley,Mode);
  encoder->init();

  BTS = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  BTS->init();

  btn= new Button(btn_pin,0);
  btn->init();


  last_t=millis();
  last_position=encoder->getPosition();
  }
   
void loop() {
    current_t=millis();
    delta_t = current_t-last_t;
    while (btn->getPress()||analogRead(pot_pin)<100)
    {
      BTS->Set_L(0);
    }
    
    if(delta_t>200){
      current_position = encoder->getPosition();
      delta_position = current_position - last_position;
      actual_vel=(delta_position/delta_t);//()mm/ms -> meter/second
      //actual_rpm=actual_vel*60/pitch_gear;//(dx/dt) RPM transform m/s -> m/min -> w=v/c
      actual_rpm=actual_vel*60/0.095;
      last_t = current_t;
      last_position=current_position;
      
      count++;
      if (current_position>10000 || current_position<-10000){ // zerar encoder para não dar overload
        encoder->setPulses(0);
        last_position=0;
      }
    }
    
    BTS->Set_L(MAX_PWM*0.5);
    count++;


   if (count>20) // printar depois de 20 ciclos só para nao printar a todo momento
      {
      Serial.println("\n"); 
      Serial.print("vel: ");
      Serial.println(actual_vel);
      Serial.print("rpm: ");
      Serial.println(actual_rpm);
      Serial.println("\n"); 
      count =0;
      }
}