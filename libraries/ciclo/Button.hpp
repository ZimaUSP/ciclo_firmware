/**
 * @file  Button.cpp
 *
 * @brief Button Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */

#ifndef __Button_HPP__
#define __Button_HPP__

#include "Arduino.h"

class Button {
    private:
        int pin;

        bool press;
        
        //nickgammon ideia
        const byte whichISR_;

        static void isr0();

        static void isr1();
   
        static void isr2();

        static void isr3();
     
     

        static Button* instance0_;
        static Button* instance1_;
        static Button* instance2_;
        static Button* instance3_;

        void handleInterrupt();
        //

    public:
        /**
         * @brief Default constructor of a Button base class
         * 
         * @param pin chave pin 
         */
        Button(int pin, const byte which);

        void init();
    
        /**
         * @brief Triggered by falling sign, demarks endstop in a bool variable
         */
        void press_trigger();
        
        /**
         * @brief get the pin number
         */
        int getPin();

        /**
         * @brief get state of movement
         */
        bool getPress();


};

#endif  // __Button_HPP__