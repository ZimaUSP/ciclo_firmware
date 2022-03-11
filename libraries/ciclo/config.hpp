#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__


/**
 * @file  config.hpp
 *
 * @brief firmware configuration
 * 
 * @copyright MIT License
 */




// State
#define STAND_BY 0
#define ACTIVE 1
#define PASSIVE 2


//  Encoder

// Comum used
#define Nominal_pulses 360
#define pitch_pulley 125
#define pitch_gear 10
#define Mode 4

#define a_pin 2 // Green cable
#define b_pin 3 // White cable


// BTS
#define MAX_PWM 50
#define r_pin 6 // L Bts
#define l_pin 5 // R Bts


// PID
#define kp  2
#define ki  0
#define kd  0.01

#endif 