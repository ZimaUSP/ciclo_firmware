/**
 * @file  Timer.hpp
 *
 * @brief Encoder class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 03/2022
 * 
 * @copyright MIT License
 */

#ifndef __Timer_HPP__
#define __Timer_HPP__

#include "Arduino.h"

class Timer {
    private:
    unsigned long init_t;
    unsigned long  period;
    unsigned long  passed_time;
    unsigned long  pause_time = 0;
    unsigned long  unpause_time = 0;
    unsigned long  last_min;
    unsigned long  last_sec;

    public:
        /**
         * @brief Default constructor of a Timer base class
         * 
         */
        Timer();
        
        /**
         * @brief inits a count down of the size of period, on minutes
         */
        void init(int period);
    
        /**
         * @brief return left to complet period (minutes part)
         */
        int current_min();

        /**
         * @brief return left to complet period (seconds part)
         */
        int current_sec();

        /**
         * @brief pause time counter
         */
        void pause();

        
        /**
         * @brief unpause time counter
         */
        void unpause();

        


};

#endif  // __ENCODER_HPP__