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
    Preferences pref;
}

void Memory::write(uint16_t* dataStore, const char* key) {
    Preferences pref;
    pref.begin(this->name_spc);
    pref.putBytes(key, (byte*)(&dataStore), sizeof(dataStore));
}

void Memory::read(uint16_t* dataRetrieve, const char* key) {
    Preferences pref;
    pref.begin(this->name_spc);
    pref.getBytes(key, &dataRetrieve, sizeof(dataRetrieve));
}

void Memory::write(double* dataStore, const char* key) {
    Preferences pref;
    pref.begin(this->name_spc);
    pref.putBytes(key, (byte*)(&dataStore), sizeof(dataStore));
}

void Memory::read(double* dataRetrieve, const char* key) {
    Preferences pref;
    pref.begin(this->name_spc);
    pref.getBytes(key, &dataRetrieve, sizeof(dataRetrieve));
}

