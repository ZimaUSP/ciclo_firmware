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
Preferences pref;
pref.begin("Memoria", false); //namespace

Memory::Memory(const char* name) {
    this->name = name;
}

void Memory::write(int* dataStore) {
    pref.putBytes(this->name, (byte*)(&dataStore), sizeof(dataStore));
}

void Memory::read(int* dataRetrieve) {
    pref.getBytes(this->name, &dataRetrieve, sizeof(dataRetrieve));
}

