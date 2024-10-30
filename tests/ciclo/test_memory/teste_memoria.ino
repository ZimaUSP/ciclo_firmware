#include "config.hpp"
#include "Memory.hpp"
#include "Preferences.h"

Memory *saved;

double dados_torque_classe[20];
double dados_torque[20];
double dados_a[20];
int dados_tempo_classe[20];
int dados_tempo[20];
int dados_b[20];
int size = 20;
const char* name_spc = "Memoria";
const char* name_spc_semclasse = "teste";
const char* key_torque = "SavedTorque";
const char* key_time = "SavedTime";
Preferences prefe;
Preferences verifica;

void setup() {
  Serial.begin(9600);
  prefe.begin(name_spc_semclasse, false);
  verifica.begin(name_spc, false);

  saved = new Memory(name_spc);//a string aqui eh o namespace
  saved->read(dados_tempo_classe, key_time, size);
  saved->read(dados_torque_classe, key_torque, size);

  prefe.getBytes(key_torque, &dados_torque, sizeof(dados_torque));
  prefe.getBytes(key_time, &dados_tempo, sizeof(dados_tempo));

  delay(2000);
  
  Serial.println();
  Serial.println("Dados na memoria do torque com classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_torque_classe[i]);
        Serial.print(" ");
  
  }
  Serial.println();
  Serial.println("Dados na memoria do tempo com classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_tempo_classe[i]);
        Serial.print(" ");
  }
  Serial.println();
  Serial.println("Dados na memoria do torque sem classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_torque[i]);
        Serial.print(" ");
  
  }
  Serial.println();
  Serial.println("Dados na memoria do tempo sem classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_tempo[i]);
        Serial.print(" ");
  }
  delay(2000);

  verifica.getBytes(key_torque, dados_a, sizeof(dados_a));
  verifica.getBytes(key_time, dados_b, sizeof(dados_b));

  Serial.println();
  Serial.println("Dados na memoria do torque sem classe, no namespace do com classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_a[i]);
        Serial.print(" ");
  
  }
  Serial.println();
  Serial.println("Dados na memoria do tempo sem classe, no namespace do com classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_b[i]);
        Serial.print(" ");
  }
  delay(5000);
  for(int i=0;i<20;i++) {
        dados_torque[i] = 0;
        dados_tempo[i] = 0;
        dados_torque_classe[i] = 0;
        dados_tempo_classe[i] = 0;
  }
  
}

void loop() {
    for(int i=0;i<20;i++) {
        dados_tempo[i] = dados_tempo[i]+i;
        dados_tempo_classe[i] = dados_tempo_classe[i]+i;
        if(i%2==0) dados_torque[i] = dados_torque[i]+0.2*i;
        if(i%2==0) dados_torque_classe[i] = dados_torque_classe[i]+0.2*i;
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
  Serial.println();
  Serial.println("Dados agora do torque com classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_torque_classe[i]);
        Serial.print(" ");
  
  }
  Serial.println();
  Serial.println("Dados agora do tempo com classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_tempo_classe[i]);
        Serial.print(" ");
  }

  saved->write(dados_torque_classe, key_torque, size);
  saved->write(dados_tempo_classe, key_time, size);

  prefe.putBytes(key_torque, dados_torque, sizeof(dados_torque));
  prefe.putBytes(key_time, dados_tempo, sizeof(dados_tempo));

  delay(1000);

for(int i=0;i<20;i++) {
        dados_torque[i] = 0;
        dados_tempo[i] = 0;
        dados_torque_classe[i] = 0;
        dados_tempo_classe[i] = 0;
  }
  delay(1000);

  saved->read(dados_torque_classe, key_torque, size);
  saved->read(dados_tempo_classe, key_time, size);

  prefe.getBytes(key_torque, dados_torque, sizeof(dados_torque));
  prefe.getBytes(key_time, dados_tempo, sizeof(dados_tempo));

  delay(2000);

  Serial.println();
  Serial.println("Dados na memoria do torque com classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_torque_classe[i]);
        Serial.print(" ");
  
  }
  Serial.println();
  Serial.println("Dados na memoria do tempo com classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_tempo_classe[i]);
        Serial.print(" ");
  }
  Serial.println();
  Serial.println("Dados na memoria do torque sem classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_torque[i]);
        Serial.print(" ");
  
  }
  Serial.println();
  Serial.println("Dados na memoria do tempo sem classe:");
  for(int i=0;i<20;i++) {
        Serial.print(dados_tempo[i]);
        Serial.print(" ");
  }
}