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
#include <String>

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/


Memory::Memory(const char* name_spc, int sessions) {
    Preferences pref;
    this->name_spc = name_spc;
    pref.begin(this->name_spc, false);
    if(pref.isKey("old")) {
        this->old = pref.getInt("old", 0);
    }
    else {
        this->old = 0;
        pref.putInt("old", 0);
    }
    this->sessions = sessions; /*sessions = number of sessions to be stored*/
    //pref.putInt("sessions", this->sessions);
    if(pref.isKey("next")) {
        this->next = pref.getInt("next", 0);
    }
    else {
        this->next = this->old;
        pref.putInt("next", this->next);
    }
    while( this->next > this->old + this->sessions ) {//if there is conflict between new number of sessions and old one
        remove_old();
    }
    pref.end();
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

void Memory::remove_old() {
    Preferences pref;
    pref.begin(this->name_spc, false);
    std::string str = std::to_string(this->old);
    if(this->name_spc == "resistivo") {
        pref.remove((str+"_torque").c_str());
        pref.remove((str+"_tempo").c_str());
    }
    else if(this->name_spc == "normal" || this->name_spc == "passivo") {
        pref.remove((str+"_frequencia").c_str());
        pref.remove((str+"_tempo").c_str());
    }
    this->old = this->old+1;
    pref.putInt("old", this->old);
    pref.end();
}

void Memory::push(int* tempo, double* lista_values, int size) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    std::string str = std::to_string(this->next);
    if(this->name_spc == "resistivo") {
        write(tempo, (str+"_tempo").c_str(), size);
        write(lista_values, (str+"_torque").c_str(), size);
    }
    else if(this->name_spc == "normal" || this->name_spc == "passivo") {
        write(tempo, (str+"_tempo").c_str(), size);
        write(lista_values, (str+"_frequencia").c_str(), size);
    }
    this->next = this->next+1;
    pref.putInt("next", this->next);
    while(this->old < this->next - this->sessions) {
        remove_old();   
    }
}

void Memory::get(int n, int* tempo, double* lista_values, int size) {
    std::string str = std::to_string(n+this->old);
    if(this->name_spc == "resistivo") {
        read(lista_values, (str+"_torque").c_str(), size);
        read(tempo, (str+"_tempo").c_str(), size);
    }
    else if(this->name_spc == "normal" || this->name_spc == "passivo") {
        read(lista_values, (str+"_frequencia").c_str(), size);
        read(tempo, (str+"_tempo").c_str(), size);
    }
}
