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
#define perimeter_pulley 125.66 // in mm
#define pitch_gear 0.596 //PITCH GEAR IN METERS
#define perimeter_gear  584.3 //in  milimeter
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

// Read vel

#define sample_t 100

#endif 