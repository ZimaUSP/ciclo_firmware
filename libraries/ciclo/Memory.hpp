/**
 * @file  Memory.cpp
 *
 * @brief Memory Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */

#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__

#include "Arduino.h"
#include "Preferences.h"

class Memory {
    private:
        const char* name_spc;
        Preferences pref;

    public:
        Memory(const char* name_spc);

        void write(uint16_t* dataStore, const char* key);

        void read(uint16_t* dataRetrieve, const char* key);

        void write(double* dataStore, const char* key);

        void read(double* dataRetrieve, const char* key);
};

#endif  // __MEMORY_HPP__
