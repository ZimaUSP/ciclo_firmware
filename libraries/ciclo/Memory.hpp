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

    public:
        Memory(const char* name_spc);

        void write(int* dataStore, const char* key, int size);

        void read(int* dataRetrieve, const char* key, int size);

        void write(double* dataStore, const char* key, int size);

        void read(double* dataRetrieve, const char* key, int size);
};

#endif  // __MEMORY_HPP__
