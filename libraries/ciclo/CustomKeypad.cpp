/**
 * @file  CustomKeypad.cpp
 *
 * @brief CustomKeypad class
 *
 * @author Guilherme Tokio Kawahara <guitokio@usp.br>
 *
 * @date 06/2025
 * 
 * @copyright MIT License
 */


#include "CustomKeypad.hpp"
/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/
CustomKeypad::CustomKeypad(byte pinos_linhas [linhas], byte pinos_colunas [colunas]) {
  for(int i = 0; i < this->linhas; i++) {
        this->pinos_linhas[i] = pinos_linhas[i];
    }
    for(int i = 0; i < this->colunas; i++) { 
        this->pinos_colunas[i] = pinos_colunas[i];
    }
  this->teclado = new Keypad(makeKeymap(TECLAS_MATRIZ), pinos_linhas, pinos_colunas, linhas, colunas);
}

char CustomKeypad::whatIsPressed() {
    char agora = teclado->getKey();
    if(agora != ultima_tecla) {
        if(teclado->isPressed(ultima_tecla))
            return ultima_tecla;
        ultima_tecla = agora;
    }
    return ultima_tecla;
}

bool CustomKeypad::left() {
    return (whatIsPressed() == charLeft);
}

bool CustomKeypad::right() {
    return (whatIsPressed() == charRight);
}

bool CustomKeypad::enter() {
    return (whatIsPressed() == charEnter);
}

bool CustomKeypad::nothingIsPressed() {
    return (whatIsPressed() == NO_KEY /*NO_KEY == '\0' */);
}
