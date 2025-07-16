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
#include "config.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/


Memory::Memory() {
    Preferences pref;
    const char* name_spc = ""; //default]
    this->name_spc = name_spc;
}

void Memory::change_namespace(const char *new_name_spc) {
    if(String(this->name_spc) == String(new_name_spc))  return;
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
        int next = pref.getInt("next", 0);
		if (next >= this->old) {
			this->next = next;
		} else {
			this->next = this->old;
		}
    }
    else {
        this->next = this->old;
        pref.putInt("next", this->next);
    }
    while( this->next > this->old + N_SESSIONS ) {//if there is conflict between new number of sessions and old one
        remove_old();
    }
    pref.end();
}

void Memory::write(int* dataStore, const char* key, int size) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    size_t len = pref.putBytes(key, (byte*)dataStore, sizeof(int)*size);
    Serial.print("Len: ");
    Serial.println(len);
    Serial.print("Size: ");
    Serial.println(size * sizeof(int));
    if(len != size * sizeof(int))
        Serial.println(" Deu ruim no int :( ");
    pref.end(); 
}

void Memory::read(int* dataRetrieve, const char* key) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    pref.getBytes(key, dataRetrieve, pref.getBytesLength(key)); 
    pref.end();
}

void Memory::write(double* dataStore, const char* key, int size) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    size_t len = pref.putBytes(key, (byte*)(dataStore), sizeof(double)*size);
    Serial.print("Len: ");
    Serial.println(len);
    Serial.print("Size: ");
    Serial.println(size * sizeof(double));
    if(len != size * sizeof(double))
        Serial.println(" Deu ruim no double :( ");
    pref.end();
}

void Memory::read(double* dataRetrieve, const char* key) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    pref.getBytes(key, dataRetrieve, pref.getBytesLength(key)); 
    pref.end();
}

void Memory::remove_old() {
    Preferences pref;
    pref.begin(this->name_spc, false);
    String str = String(this->old);
    pref.remove((str+"_values").c_str());
    pref.remove((str+"_tempo").c_str());
    this->old = this->old+1;
    pref.putInt("old", this->old);
    pref.end();
}

void Memory::push(int* tempo, double* lista_values, int size) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    String str = String(this->next);
    write(tempo, (str+"_tempo").c_str(), size);
    write(lista_values, (str+"_values").c_str(), size);
    this->next = this->next+1;
    pref.putInt("next", this->next);
    while(this->old < this->next - N_SESSIONS) {
        remove_old();   
    }
    pref.end();
}

 String Memory::printalgo(int n){
    String str = String(n+this->old);
    return((str+"_values").c_str());
}

void Memory::get(int n, int* tempo, double* lista_values) {
    String str = String(n+this->old);
    read(lista_values, (str+"_values").c_str());
    read(tempo, (str+"_tempo").c_str());
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

int Memory::size(int session_num) {
    Preferences pref;
    pref.begin(this->name_spc, false);
    String str = String(session_num);
    int size = pref.getBytesLength((str + "_tempo").c_str());
    size /= sizeof(int);
    pref.end();
    return size;
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
