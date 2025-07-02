#include "CustomKeypad.hpp"

CustomKeypad* Keypad;

byte linhas[4] = {13, 12, 14, 27}; //pinos a serem definidos
byte colunas[4] = {26, 25, 33, 32}; //pinos a serem definidos

void setup() {
  Serial.begin(9600); 
  Keypad = new CustomKeypad(linhas, colunas); 
}

void loop() {

  Serial.print("left: ");
  Serial.println(Keypad->left() ? "true" : "false");
  Serial.print("rigth: ");
  Serial.println(Keypad->right() ? "true" : "false");
  Serial.print("enter: ");
  Serial.println(Keypad->enter() ? "true" : "false");
  delay(500);

}