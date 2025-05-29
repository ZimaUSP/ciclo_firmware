#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__


/**
 * @file  config.hpp
 *
 * @brief firmware configuration
 * 
 * @copyright MIT License
 */

 // Para o modo assistido Usar pwm em R
 // Para o modo resistido Usar pwm em L


// State
#define STAND_BY 0
#define NORMAL 1
#define FADE 2
#define PASSIVE 3
#define MODE 4
#define TIMER 5
#define SET_VEL 6
#define SET_DRAG 7
#define DONE 8
#define RESET 9
#define CONFIG 4
#define GETIP 0
#define RESETWIFI 1
#define RESETESP 2
#define BACK 3


#define WEEK 0
#define NORMAL 1
#define STRONG 2

// Mode
#define MANUAL 0
#define AUTO 1

//reset constant
#define RESET_CMD asm ("jmp (0x0000)")


//  Encoder

#define a_pin 16 // Green cable
#define b_pin 17 // White cable


// Comum used
#define Nominal_pulses 360 // pulsos por volta no encoder
#define pulses_per_rev 1650 // pulsos por volta do ciclo
#define MAX_ENCODER_VALUE 10000
#define Mode 1
#define PWM_frequency_channel 10000
#define PWM_resolution_channel 8
#define tolerance 5

//Channel

#define R_channel 1
#define L_channel 2


// BTS
#define MAX_PWM 50
#define max_pwm_cte 0.7
#define r_pin 23 
#define l_pin 19 


// PID
#define kp  1.4
#define ki  0.008
#define kd  0
#define i_saturation 10000

//Rele
#define stand_by_active 9
#define passive_active 10

//Potentiometer
#define pot_pin 34 // 15 - changed manually

//Button
#define btn_pin 4 

//Enable
#define enable_pin 33

// Fade
#define fade 50

// current sensor
#define acs_pin 36
#define acs_max_amp 20 

// Parametros de NUMERO DE CICLOS DADOS NO PERIODO TESTE diagnostico da saude do paciente
#define sample_ex 1
#define LIMITE_DEBILIDADO 5
#define LIMITE_SAUDAVEL 10

#define sample_t 400 // min value possible - to recalibrate that use debug on odometry_calc to se delta_t
#endif 
