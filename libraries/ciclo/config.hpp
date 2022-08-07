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
#define ACTIVE_PLUS 1
#define FADE 2
#define PASSIVE 3
#define MODE 4
#define TIMER 5
#define SET_VEL 6
#define SET_DRAG 7
#define DONE 8
#define RESET 9

#define WEEK 0
#define NORMAL 1
#define STRONG 2

// Mode
#define MANUAL 0
#define AUTO 1

//reset constant
#define RESET_CMD asm ("jmp (0x0000)")


//  Encoder

// Comum used
#define Nominal_pulses 360
#define perimeter_pulley 125.66 // in mm
#define pitch_gear 0.596 //PITCH GEAR IN METERS
#define perimeter_gear  584.3 //in  milimeter
#define Mode 1
#define PWM_frequency_channel 40000
#define PWM_resolution_channel 8


#define a_pin 3 // White cable
#define b_pin 2 // Green cable


// BTS
#define MAX_PWM 50
#define r_pin 6 // L Bts
#define l_pin 5 // R Bts
#define R_channel 0
#define L_channel 1 

// PID
#define kp  5
#define ki  0.01
#define kd  0

//Rele
#define stand_by_active 9
#define passive_active 10

//Potentiometer
#define pot_pin A0

//Button
#define btn_pin 11

// Fade
#define fade 50

// current sensor
#define acs_pin 36
#define acs_max_amp 20 

// Parametros de NUMERO DE CICLOS DADOS NO PERIODO TESTE diagnostico da saude do paciente
#define sample_ex 1
#define LIMITE_DEBILIDADO 5
#define LIMITE_SAUDAVEL 10

#define sample_t 38 // min value possible - to recalibrate that use debug on odometry_calc to se delta_t
#endif 