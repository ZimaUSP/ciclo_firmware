/**
 * @file  CustomKeypad.cpp
 *
 * @brief CustomKeypad Class
 *
 * @author Guilherme Tokio Kawahara <guitokio@usp.br>
 *
 * @date 06/2025
 * 
 * @copyright MIT License
 */

#ifndef __CUSTOMKEYPAD_HPP__
#define __CUSTOMKEYPAD_HPP__

#include "Arduino.h"
#include "Keypad.h" 

class CustomKeypad {
    private:
        const byte linhas = 4; // Linhas do teclado
        const byte colunas = 4; // Colunas do teclado
        const char TECLAS_MATRIZ[linhas][colunas] = { // Matriz de caracteres (mapeamento do teclado)
            {'1', '2', '3', 'A'},
            {'4', '5', '6', 'B'},
            {'7', '8', '9', 'C'},
            {'*', '0', '#', 'D'}
        };
        const char charRight = '*', charLeft = '#', charEnter = '0'; //placeholder
        byte pinos_linhas [linhas];
        byte pinos_colunas [colunas];
        char ultima_tecla = '\0';
        Keypad* teclado;

    public:
        CustomKeypad(byte pinos_linas [linhas], byte pinos_colunas [colunas]);

        char whatIsPressed();

        bool right();

        bool left();

		bool enter();

        bool nothingIsPressed(); /*Equivalente ao middle() do joystick*/
};

#endif  // __CUSTOMKEYPAD_HPP__
