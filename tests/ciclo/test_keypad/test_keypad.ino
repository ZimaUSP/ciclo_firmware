#include "CustomKeypad.hpp"

CustomKeypad* Keypad;

byte linhas[4] = {9, 8, 7, 6}; //pinos a serem definidos
byte colunas[4] = {5, 4, 3, 2}; //pinos a serem definidos

void setup() {
  Serial.begin(9600); 
  Keypad = new CustomKeypad(linhas, colunas); 
}

void loop() {

  Serial.println("O que esta sendo pressionado agora eh: ");
  Serial.print(Keypad->whatIsPressed());
  delay(500);

}