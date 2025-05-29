
#ifndef NORMAL_HPP
#define NORMAL_HPP

#include "Arduino.h"
#include "Preferences.h"

class normal
{
private:
    int contador;
    int tempo[];
    double lista_values[];
    LiquidCrystal_I2C lcd;
    SimpleTimer rpmTime;
    SimpleTimer lcd_timer;
    Encoder* encoder;
    H_bridge_controller motor;
    
public:
    normal(int contador,int tempo,double lista_values,LiquidCrystal_I2C lcd, SimpleTimer rpmTime,SimpleTimer lcd_timer, encoder, H_bridge_controller motor);
    void executaNormal();
    void resetaEncoder(double current_pulses,double last_pulses);
};
