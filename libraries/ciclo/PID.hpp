/**
 * @file  PID.cpp
 *
 * @brief PID Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */

#ifndef __PID_HPP__
#define __PID_HPP__

#include "Arduino.h"

class PID {
    protected:

        unsigned long current_time;
        unsigned long previus_time = 0;
        long delta_time;

        
        long i_error =0;
        float d_error;

        float k_p;
        float k_i;
        float k_d;

        float error;
        float previus_error;

    public:
        /**
         * @brief Default constructor of a PID base class
         * 
         * @param k_p Proporcional constant 
         * @param k_i Integrative constant 
         * @param k_d Derivative constant 
         */
        PID(float k_p,float k_i,float k_d);

        /**
         * @brief Computes PID value
         * 
         * @param Input input value 
         * @param setpoint Set goal, the value that input should be
         */
        float computePID(float input,float setpoint); 

        /**
         * @brief Reset PID values
         * 
         */
        void reset();


};

#endif  // __PID_HPP__