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
#define FADE 2
#define PASSIVE 3


//  Encoder

// Comum used
#define Nominal_pulses 360
#define perimeter_pulley 125.66 // in mm
#define pitch_gear 0.596 //PITCH GEAR IN METERS
#define perimeter_gear  584.3 //in  milimeter
#define Mode 1

#define a_pin 3 // White cable
#define b_pin 2 // Green cable


// BTS
#define MAX_PWM 50
#define r_pin 6 // L Bts
#define l_pin 5 // R Bts


// PID
#define kp  5
#define ki  0.01
#define kd  0

//Rele
#define stand_by_active 9
#define passive_active 10

//Potentiometer
#define pot_pin A0

// Fade
#define fade 50

#define sample_t 38 // min value possible - to recalibrate that use debug on odometry_calc to se delta_t
#endif 