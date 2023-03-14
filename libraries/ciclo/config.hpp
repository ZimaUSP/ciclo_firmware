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

#define a_pin 17 // White cable
#define b_pin 16 // Green cable


// Comum used
#define Nominal_pulses 360 // pulsos por volta no encoder
#define perimeter_pulley 125.66 // raio da coroa do encoder in mm
#define pitch_gear 0.596 //perimetro da coroa do pedal PITCH GEAR IN METERS
#define perimeter_gear  584.3 // valor realculado impiricamente in  milimeter
#define Mode 1
#define PWM_frequency_channel 10000
#define PWM_resolution_channel 8
#define tolerance 5




// BTS
#define MAX_PWM 255
#define max_pwm_cte 0.7
#define r_pin 23 
#define l_pin 19 
#define R_channel 0
#define L_channel 1 

// PID
#define kp  10
#define ki  0.01
#define kd  0
#define i_saturation 1000

//Rele
#define stand_by_active 9
#define passive_active 10

//Potentiometer
#define pot_pin 15

//Button
#define btn_pin 4 

// Fade
#define fade 50

// current sensor
#define acs_pin 36
#define acs_max_amp 20 

// Parametros de NUMERO DE CICLOS DADOS NO PERIODO TESTE diagnostico da saude do paciente
#define sample_ex 1
#define LIMITE_DEBILIDADO 5
#define LIMITE_SAUDAVEL 10

#define sample_t 50 // min value possible - to recalibrate that use debug on odometry_calc to se delta_t
#endif 