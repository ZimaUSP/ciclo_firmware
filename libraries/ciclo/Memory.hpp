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
        int old, next, sessions, limitpush;

    public:
        Memory();

        void change_namespace(const char *new_name_spc);

        void write(int* dataStore, const char* key, int size);

        void read(int* dataRetrieve, const char* key);

        void write(double* dataStore, const char* key, int size);

        void read(double* dataRetrieve, const char* key);

        void remove_old();

        String printalgo(int n);

        void push(int* tempo, double* lista_values, int size);

        void get(int n, int* tempo, double* lista_values);

        void push_resistivo(int* tempo, double* lista_values, int size);

        void push_normal(int* tempo, double* lista_values, int size);

        void push_passivo(int* tempo, double* lista_values, int size);

        void get_resistivo(int n, int* tempo, double* lista_values);
        
        void get_normal(int n, int* tempo, double* lista_values);
        
        void get_passivo(int n, int* tempo, double* lista_values);

        int get_saved_sessions_resistivo();/*return number of sessions saved in resistivo*/
        
        int get_saved_sessions_normal();
        
        int get_saved_sessions_passivo();

        int size(int sessions_num);

        int size_resistivo(int session_num);/*return size, number of measurements, of the nth session in resistivo*/
        
        int size_normal(int session_num);
        
        int size_passivo(int session_num);

};

#endif  // __MEMORY_HPP__