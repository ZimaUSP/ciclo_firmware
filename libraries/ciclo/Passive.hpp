#ifndef __PASSIVE_HPP__
#define __PASSIVE_HPP__

#include <SimpleTimer.h>
#include <Arduino.h>
#include "Encoder.hpp"
#include "config.hpp"
#include "Memory.hpp"
#include "PID.hpp"
#include "H_bridge_controller.hpp"
#include <LiquidCrystal_I2C.h>

#define MAX_SAMPLES 5

class Passive {
    private:
    
    LiquidCrystal_I2C* lcd;
    SimpleTimer lcd_timer;
    SimpleTimer rpmTime;
    Encoder* encoder;
    Memory* saved;
    PID* PID_vel;
    H_bridge_controller* motorController;

    char t[10]; 
    int contador;
    int output;
    double lista_values [MAX_SAMPLES];
    int tempo [MAX_SAMPLES];
    
    double current_pulses;
    double last_pulses;
    double delta_pulses;
    float actual_rpm;
    int goal_rpm;
    
    void reset_mode();
    void new_torques();
    void printTime();
    
    public:
    Passive(SimpleTimer lcd_timer, SimpleTimer rpmTimer, Encoder* encoder, Memory* saved, PID* PID_vel, H_bridge_controller* motorController, LiquidCrystal_I2C* lcd);
    void controlMotorSpeedWithPID();
    void resetEncoderIfExceedsLimit();
    void init_passive();
};

#endif // __PASSIVE_HPP__