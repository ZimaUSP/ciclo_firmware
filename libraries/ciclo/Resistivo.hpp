// CLASSE

#ifndef __RESISTIVO_HPP__
#define __RESISTIVO_HPP__

#include <SimpleTimer.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Encoder.hpp"
#include "config.hpp" 
#include "Memory.hpp"
#include "PID.hpp"
#include "H_bridge_controller.hpp"


#include "Button.hpp"
#include "Joystick.hpp"
#include "current_sensor.hpp"

class resistivo {
private:
    
    LiquidCrystal_I2C* lcd;
    SimpleTimer* lcd_timer;
    SimpleTimer* rpmTime;
    SimpleTimer* torque_Time; 
    Encoder* encoder;
    Memory* saved;
    PID* PID_vel;
    H_bridge_controller* motorController;
    Button* btn;
    Joystick* joy;
    current_sensor* cur;

    
    double t0, torque, torque_max, torque_min, torque_med;
    float acs;
    char t[10];
    int offset, pot, sum, i, contador, pwm_motor;
    double  t_Duration = 0.5; 
    int verif = 1;
    int n_sessions;
    double lista_values[MAX_SAMPLES];
    int tempo[MAX_SAMPLES];
    bool joystick_check;

    
    int verificationResistivo();
    int duration();
    void printTime();

public:
    // Construtor 
    resistivo(LiquidCrystal_I2C* lcd, SimpleTimer* lcd_timer, SimpleTimer* rpmTime, SimpleTimer* torque_Time,
        Encoder* encoder, Memory* saved, PID* PID_vel, H_bridge_controller* motorController,
        Button* btn, Joystick* joy, current_sensor* cur);

    
    void executarLogicaResistivo();
    int def_pwm_motor();
    void resistivo(); 
    void print_torque_results();
};

#endif 