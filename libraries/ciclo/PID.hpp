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
    private:

        unsigned long current_time;
        unsigned long previous_time;
        double delta_time;

        double integrative_term;
        double d_error;
        double input_error;

        double k_p;
        double k_i;
        double k_d;

        double proportional;
        double integrative;
        double derivative;

        double error;
        static const int INPUT_AVG_BUFFER_SIZE = 5;
        double previous_inputs[INPUT_AVG_BUFFER_SIZE];
        int input_index;
        int input_count;
        double previous_input_avg;
        double previous_input;
        double previous_error;
        double previous_output;

        int wind_up_saturation;
    public:
        /**
         * @brief Default constructor of a PID base class
         * 
         * @param k_p Proporcional constant 
         * @param k_i Integrative constant 
         * @param k_d Derivative constant 
         */
        PID(double k_p,double k_i,double k_d,int i_saturation);

        /**
         * @brief Computes PID value
         * 
         * @param Input input value 
         * @param setpoint Set goal, the value that input should be
         */
        double computePID(double input,float setpoint,float tolerance); //, double& error, double& integrative_term, double& d_error); 

        /**
         * @brief Reset PID values
         * 
         */
        void reset();

        void imprimir();

};

#endif  // __PID_HPP__