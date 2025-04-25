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
#include <string>

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/


Memory::Memory(int sessions) {
    Preferences pref;
    const char* name_spc = "resistivo"; //default
    this->limitpush = 10 * 60 * sessions/5;
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

void Memory::change_namespace(const char *new_name_spc) {
    if(std::string(this->name_spc) == std::string(new_name_spc))  return;
    Preferences pref;
    this->name_spc = new_name_spc;
    pref.begin(this->name_spc, false);
    if(pref.isKey("old")) {
        this->old = pref.getInt("old", 0);
    }
    else {
        this->old = 0;
        pref.putInt("old", 0);
    }
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

void Memory::vetorcomsize(int*& dataStore, int size) {
    int* novodataStore = new int[size + 1];
    novodataStore[0] = size;
    for (int i = 0; i < size; i++){
        novodataStore[i+1] = dataStore[i]; 
    }
    dataStore = novodataStore;
}

void Memory::vetorcomsize(double*& dataStore, int size) {
    double* novodataStore = new double[size + 1];
    novodataStore[0] = size;
    for (int i = 0; i < size; i++){
        novodataStore[i+1] = dataStore[i]; 
    }
    dataStore = novodataStore;
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
    pref.remove((str+"_values").c_str());
    pref.remove((str+"_tempo").c_str());
    this->old = this->old+1;
    pref.putInt("old", this->old);
    pref.end();
}

void Memory::push(int* tempo, double* lista_values, int size) {
    Preferences pref;
    if(this->next > this->limitpush){
        return;
    }
    pref.begin(this->name_spc, false);
    std::string str = std::to_string(this->next);
    vetorcomsize(tempo, size);
    vetorcomsize(lista_values, size);
    write(tempo, (str+"_tempo").c_str(), size + 1);
    write(lista_values, (str+"_values").c_str(), size + 1);
    this->next = this->next+1;
    pref.putInt("next", this->next);
    while(this->old < this->next - this->sessions) {
        remove_old();   
    }
    pref.end();
}

 String Memory::printalgo(int n){
    std::string str = std::to_string(n+this->old);
    return((str+"_values").c_str());
}

void Memory::get(int n, int* tempo, double* lista_values) {
    std::string str = std::to_string(n+this->old);
    int s = size(n);
    if(n + this->old > limitpush){
        for(int i = 0; i < 5; i++ ){
            tempo[i] = 0;
            lista_values[i] = 0;
        }
        return;
    }
    read(lista_values, (str+"_values").c_str(), s);
    read(tempo, (str+"_tempo").c_str(), s);
}

int Memory::size(int n) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    std::string str = std::to_string(n + this->old);
    return pref.getBytesLength((str+"_tempo").c_str())/sizeof(int);
}

void Memory::push_resistivo(int* tempo, double* lista_values, int size) {
    change_namespace("resistivo");
    push(tempo, lista_values, size);
}

void Memory::push_normal(int* tempo, double* lista_values, int size) {
    change_namespace("normal");
    push(tempo, lista_values, size);
}

void Memory::push_passivo(int* tempo, double* lista_values, int size) {
    change_namespace("passivo");
    push(tempo, lista_values, size);
}

void Memory::get_resistivo(int n, int* tempo, double* lista_values) {
    change_namespace("resistivo");
    get(n, tempo, lista_values);
}
        
void Memory::get_normal(int n, int* tempo, double* lista_values) {
    change_namespace("normal");
    get(n, tempo, lista_values);
}
        
void Memory::get_passivo(int n, int* tempo, double* lista_values) {
    change_namespace("passivo");
    get(n, tempo, lista_values);
}

int Memory::get_saved_sessions_resistivo() {
    change_namespace("resistivo");
    return (this->next - this->old);
}
        
int Memory::get_saved_sessions_normal() {
    change_namespace("normal");
    return (this->next - this->old);
}
        
int Memory::get_saved_sessions_passivo() {
    change_namespace("passivo");
    return (this->next - this->old);
}

int Memory::size_resistivo(int session_num) {
    change_namespace("resistivo");
    return size(session_num);
}
        
int Memory::size_normal(int session_num) {
    change_namespace("normal");
    return size(session_num);
}
        
int Memory::size_passivo(int session_num) {
    change_namespace("passivo");
    return size(session_num);
}
