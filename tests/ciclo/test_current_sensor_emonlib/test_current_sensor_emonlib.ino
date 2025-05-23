#include "EmonLib.h" //INCLUSÃO DE BIBLIOTECA
#include "H_bridge_controller.hpp"
#include "config.hpp"
#include <LiquidCrystal_I2C.h>
 
H_bridge_controller *Motor;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define CURRENT_CAL 9 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO MEDINDO A CORRENTE DA CARGA)
const int pinoSensor = acs_pin; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO
float ruido = 0.08; //RUÍDO PRODUZIDO NA SAÍDA DO SENSOR (DEVE SER AJUSTADO COM A CARGA DESLIGADA APÓS CARREGAMENTO DO CÓDIGO NO ARDUINO)
int pwm = 100;
bool joystick_check;

EnergyMonitor emon1; //CRIA UMA INSTÂNCIA
 
void setup(){  
  Serial.begin(9600); //INICIALIZA A SERIAL
  emon1.current(pinoSensor, CURRENT_CAL); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO)
  Motor= new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  Motor->init();
  Motor->Set_R(0);
  lcd.init();
}
 
void loop(){
  //pot = analogRead(pot_pin);
  //pwm = map(pot,0,4095,0,200); //50 100 130 160 190
/*/
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
      }
      /*/
  Motor->Set_L(pwm);

  emon1.calcVI(17,100); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  double currentDraw = emon1.Irms; //VARIÁVEL RECEBE O VALOR DE CORRENTE RMS OBTIDOx
  currentDraw = currentDraw-ruido; //VARIÁVEL RECEBE O VALOR RESULTANTE DA CORRENTE RMS MENOS O RUÍDO
  
  if(currentDraw < 0){ //SE O VALOR DA VARIÁVEL FOR MENOR QUE 0, FAZ 
      currentDraw = 0; //VARIÁVEL RECEBE 0
  }
    Serial.print("Corrente medida: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(currentDraw); //IMPRIME NA SERIAL O VALOR DE CORRENTE MEDIDA
    Serial.println("A"); //IMPRIME O TEXTO NA SERIAL
    //Serial.print("pwm: ");
    //Serial.println(pwm);    
    lcd.setCursor(0, 0);
    lcd.print("Corrente: ");
    lcd.print(currentDraw);
  

}