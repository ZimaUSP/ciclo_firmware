
/*#ifndef NORMAL_HPP
#define NORMAL_HPP

#include "Arduino.h"
#include "Preferences.h"

class normal
{
private:
    int contador;
    int *tempo;
    double *lista_values;
    LiquidCrystal_I2C lcd;
    SimpleTimer rpmTime;
    SimpleTimer lcd_timer;
    Encoder* encoder;
    H_bridge_controller *motor;
    
public:
    normal(int contador,int *tempo,double *lista_values,LiquidCrystal_I2C lcd, SimpleTimer rpmTime,SimpleTimer lcd_timer, Encoder *encoder, H_bridge_controller *motor);
    void executaNormal();
    void resetaEncoder(double current_pulses,double last_pulses);
};
#endif */

#ifndef NORMAL_HPP
#define NORMAL_HPP

#include "Arduino.h"
#include "Preferences.h"
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>
#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "Memory.hpp"
#include "config.hpp"

class normal
{
private:
    int contador;
    int *tempo;
    double *lista_values;
    LiquidCrystal_I2C& lcd;  // Reference to LCD
    SimpleTimer& rpmTime;    // Reference to timer
    SimpleTimer& lcd_timer;  // Reference to timer
    Encoder* encoder;
    H_bridge_controller* motor;
    Memory* saved;
    int sample_time;
    
public:
    normal(int contador, int *tempo, double *lista_values, LiquidCrystal_I2C &lcd, 
           SimpleTimer &rpmTime, SimpleTimer &lcd_timer, Encoder *encoder, 
           H_bridge_controller *motor, Memory *saved, int sample_time);
    void executaNormal();
    void resetaEncoder(double current_pulses, double last_pulses);
    void printTime();
};

#endif