#include "config.hpp"
#include "Memory.hpp"

Memory *mem_torque;
Memory *mem_time;

int dados_torque[20];
int dados_tempo[20];

void setup() {
  Serial.begin(9600);
  mem_torque = new Memory("SavedTorque");//a string "SavedTorque" eh a chave
  mem_time = new Memory("SavedTime");
  for(int i=0;i<20;i++) {
        dados_torque[i] = 0;
        dados_tempo[i] = 0;
  }
  mem_torque.read(dados_torque);
  mem_time.read(dados_tempo);
  Serial.println("Dados na memoria do torque:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_torque[i]);
        Serial.print(" ");
  
  }
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
    Serial.println("Dados agora do torque:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_torque[i]);
        Serial.print(" ");
  
  }
  Serial.println("Dados agora do tempo:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_tempo[i]);
        Serial.print(" ");
  }
  mem_torque.write(dados_torque);
  mem_time.write(dados_tempo);
  delay(1000);
}