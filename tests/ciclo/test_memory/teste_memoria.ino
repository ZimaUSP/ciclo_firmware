#include "config.hpp"
#include "Memory.hpp"

Memory *saved;

uint16_t dados_torque[20];
uint16_t dados_tempo[20];
const char* name_spc = "Memoria";
const char* key_torque = "SavedTorque";
const char* key_time = "SavedTime";

void setup() {
  Serial.begin(9600);
  saved = new Memory(name_spc);//a string aqui eh o namespace
  for(int i=0;i<20;i++) {
        dados_torque[i] = 0;
        dados_tempo[i] = 0;
  }
  saved->read(dados_torque, key_torque);
  saved->read(dados_tempo, key_time);
  Serial.println();
  Serial.println("Dados na memoria do torque:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_torque[i]);
        Serial.print(" ");
  
  }
  Serial.println();
  Serial.println("Dados na memoria do tempo:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_tempo[i]);
        Serial.print(" ");
  }
  for(int i=0;i<20;i++) {
        dados_torque[i] = 0;
        dados_tempo[i] = 0;
  }
}

void loop() {
    for(int i=0;i<20;i++) {
        dados_tempo[i] = dados_tempo[i]+1;
        if(i%2==0) dados_torque[i] = dados_torque[i]+1;
  }
  Serial.println();
    Serial.println("Dados agora do torque:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_torque[i]);
        Serial.print(" ");
  
  }
  Serial.println();
  Serial.println("Dados agora do tempo:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_tempo[i]);
        Serial.print(" ");
  }
  saved->write(dados_torque, key_torque);
  saved->write(dados_tempo, key_torque);
  delay(1000);
}