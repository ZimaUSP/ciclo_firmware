/**
 * @file  current_sensor.cpp
 *
 * @brief Encoder class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 05/2022
 * 
 * @copyright MIT License
 */


#include "Memory.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/


Memory::Memory(const char* name_spc) {
    this->name_spc = name_spc;
}

void Memory::write(int* dataStore, const char* key, int size) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    pref.putBytes(key, (byte*)dataStore, sizeof(int)*size);
    pref.end(); 
}

void Memory::read(int* dataRetrieve, const char* key, int size) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    pref.getBytes(key, dataRetrieve, sizeof(int)*size);
    pref.end();
}

void Memory::write(double* dataStore, const char* key, int size) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    pref.putBytes(key, (byte*)(dataStore), sizeof(double)*size);
    pref.end();
}

void Memory::read(double* dataRetrieve, const char* key, int size) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    pref.getBytes(key, dataRetrieve, sizeof(double)*size);
    pref.end();
}

