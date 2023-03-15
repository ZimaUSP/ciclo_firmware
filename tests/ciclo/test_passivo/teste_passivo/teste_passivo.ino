#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "PID.hpp"
#include "config.hpp"
#include "Button.hpp"

Encoder *encoder;
H_bridge_controller *BTS;
Button *btn;

PID *PID_vel; 
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
int count =0;
 
void setup() {     
  Serial.begin (9600);

  encoder = new Encoder(a_pin,b_pin,0,Nominal_pulses,125,Mode);
  encoder->init();

  BTS = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  BTS->init();

  PID_vel = new PID(1.4,0.008,kd,i_saturation);
 
  btn= new Button(btn_pin,3);
 
  last_t=millis();
  last_pulses =encoder->getPulses();
  }
   
void loop() {
    current_t=millis();
    delta_t = current_t - last_t;

    while(analogRead(pot_pin)<100)
    {
      BTS->Set_L(0);
    }
    
    if(delta_t>sample_t){
      current_pulses = encoder->getPulses(); //t1
      delta_pulses = current_pulses - last_pulses  ;//t1-t0

      delta_ciclos = delta_pulses/pulses_per_rev;

      actual_rpm = delta_ciclos*60000/sample_t;

      last_t=current_t;
      last_pulses=current_pulses;
      
      if (current_pulses>10000 || current_pulses<-10000){ // zerar encoder para não 
        encoder->setPulses(0);
        last_pulses=0;

      }
      

    goal_rpm = map(analogRead(pot_pin), 0, 4095,0,50); // rpm
   Serial.println(goal_rpm);
    // PID_vel

    output = PID_vel->computePID(actual_rpm,goal_rpm,tolerance);
    
    delay(1);

    // Setting direction of motion acording to output_x PID
    if (output < 0) {
        if (output < -MAX_PWM) {
          output = -MAX_PWM;
        }
        BTS->Set_L(-output);
        return;
      } else {
        if (output > MAX_PWM) {
          output = MAX_PWM;
        }
        BTS->Set_R(output);
        return;
      }

  }
}