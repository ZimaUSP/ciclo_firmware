
#include<LiquidCrystal_I2C.h>
#include<SimpleTimer.h>
#include<Encoder.hpp>
#include<H_bridge_controller.hpp>

class passivo
{
public:
    int contador;
    int tempo[];
    double lista_values[];
    LiquidCrystal_I2C display;
    SimpleTimer rpmTimer;
    Encoder enc;
    H_bridge_controller motor;
}

passivo::passivo(int contador,int tempo,double lista_values,LiquidCrystal_I2C display, SimpleTimer rpmTimer,Encoder enc, H_bridge_controller motor){
    this->contador = contador;
    this->display = display;
    this->enc =  enc;
    this->rpmTimer = rpmTimer;
    this->motor = motor;
}

void passivo::resetaTorque(){
    for (int i = 0; i < MAX_SAMPLES; i++) {
        lista_values[i] = 3.0;
        tempo[i]=3;
    }
}

void passivo::executaPassivo(){

    while (!display.lcdTimer.IsRead()) {
        if (enc.rpmTimer.getTimePassed() > sample_t) {
          current_pulses = enc->getPulses();
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
    
}


/*


void passivo() {
  contador = 0;
  lcd_timer.reset();
  rpmTime.reset();
  
  // Resetar o array de torques
  for (int i = 0; i < MAX_SAMPLES; i++) {
      lista_values[i] = 3.0;
      tempo[i]=3;
  }

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


*/