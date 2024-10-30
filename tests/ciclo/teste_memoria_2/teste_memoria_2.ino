#include "config.hpp"
#include "Memory.hpp"
Memory *saved;

double dados_torque[20];
int dados_tempo[20];

int size = 5;
const char* name_spc = "resistivo";
const char* key_torque = "SavedTorque";
const char* key_time = "SavedTime";



void setup() {
  Serial.begin(9600);

  saved = new Memory(name_spc);//a string aqui eh o namespace

  saved->read(dados_torque, key_torque, size);
  saved->read(dados_tempo, key_time, size);
  
  
  Serial.println();
  Serial.println("Dados na memoria do torque sem classe:");
  for(int i=0;i<5;i++) {
        Serial.print(dados_torque[i]);
        Serial.print(" ");
  
  }
  Serial.println();
  Serial.println("Dados na memoria do tempo sem classe:");
  for(int i=0;i<5;i++) {
        Serial.print(dados_tempo[i]);
        Serial.print(" ");
  }
  
  //int tempo_write [5] = {0, 1, 2, 3, 4};
  //double torque_write [5] = {20, 10, 13, 5, 17};
  //saved->write(torque_write, key_torque, size);
  //saved->write(tempo_write, key_time, size);
  
}

void loop() {

}