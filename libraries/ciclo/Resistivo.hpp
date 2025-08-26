// CLASSE

#ifndef __RESISTIVO_HPP__
#define __RESISTIVO_HPP__


#include <SimpleTimer.h>
#include <Arduino.h>
#include "Encoder.hpp"
#include "config.hpp"
#include "Memory.hpp"
#include "PID.hpp"
#include "H_bridge_controller.hpp"
#include <LiquidCrystal_I2C.h>

/*
TRECHO DO MAIN CICLO
//resistivo
double t0, torque, torque_max, torque_min, torque_med;
float acs;
char t[10];  // Aumentado o tamanho do buffer de sprintf
int offset, pot, sum, i, contador,pwm_motor;
double  t_Duration = 0.5;
int verif = 1;
int n_sessions;
double lista_values [MAX_SAMPLES];
int tempo [MAX_SAMPLES];

//Parametros para joystick
uint32_t adc_register;
uint32_t wifi_register;

//Parametros para website
WebServer server(80);  
WiFiManager wm; //objeto da classe wifimanager
const char* MDNSDOMAIN = "ciclo";

*/

// Funções: executarLogicaResistivo(), int def_pwm_motor(), resistivo(), print_torque_results()


class resistivo {
    private:

    LiquidCrystal_I2C* lcd;
    SimpleTimer lcd_timer;
    SimpleTimer rpmTime;
    Encoder* encoder;
    Memory* saved;
    PID* PID_vel;
    H_bridge_controller* motorController;


    double t0, torque, torque_max, torque_min, torque_med;
    float acs;
    char t[10];
    int offset, pot, sum, i, contador,pwm_motor;
    double  t_Duration = 0.5;
    int verif = 1;
    int n_sessions;
    double lista_values [MAX_SAMPLES];
    int tempo [MAX_SAMPLES];


    public:

    resistivo(LiquidCrystal_I2C* lcd, SimpleTimer lcd_timer, SimpleTimer rpmTime, Encoder* encoder, Memory* saved,
PID* PID_vel, H_bridge_controller* motorController);

    void executarLogicaResistivo();

    int def_pwm_motor();

    void resistivo();
    
    void print_torque_results();
    
};

#endif