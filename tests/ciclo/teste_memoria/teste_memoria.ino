#include "config.hpp"
#include "Memory.hpp"
Memory *saved;

double dados_torque [20];
int dados_tempo [20];

int size = 5;
int sessions = 8;
const char* name_spc = "resistivo";
const char* key_torque = "SavedTorque";
const char* key_time = "SavedTime";

int tempo_write0[5] = {0, 1, 2, 3, 4};
  int tempo_write1[5] = {1, 7, 1, 4, 1};
  int tempo_write2[5] = {2, 1, 2, 3, 4};
  int tempo_write3[5] = {3, 1, 2, 7, 4};
  int tempo_write4[5] = {3, 1, 2, 7, 4};
  double torque_write0[5] = {20, 10, 13, 5, 17};
  double torque_write1[5] = {0.2, 10, 13, 5, 17};
  double torque_write2[5] = {1.2, 10, 13, 5, 17};
  double torque_write3[5] = {4.87, 10, 13, 5, 17};
  double torque_write4[5] = {3.4, 10, 13, 5, 17};

  int tempo_push[5] = {32, 32, 32, 32, 32};
  double torque_push[5] = {0.2, 0.3, 0.4, 0.5, 0};



void setup() {
  Serial.begin(9600);

  saved = new Memory(name_spc, sessions);//a string aqui eh o namespace

  //saved->read(dados_torque, key_torque, size);
  //saved->read(dados_tempo, key_time, size);
  
  //saved->write(torque_write, key_torque, size);
  //saved->write(tempo_write, key_time, size);
    saved->push_resistivo (tempo_write0, torque_write0, size);
    saved->push_resistivo (tempo_write1, torque_write1, size);
    saved->push_resistivo (tempo_write2, torque_write2, size);
    saved->push_resistivo (tempo_write3, torque_write3, size);
    saved->push_resistivo (tempo_write4, torque_write4, size);
}

void loop() {
  
  for(int i=0;i<size;i++) {
    int pega_tempo[5];
    double pega_torque[5];
    for(int i=0;i<size;i++) {
      pega_tempo[i] = 1;
      pega_torque[i] = 1;
    }
    saved->get_resistivo(i, pega_tempo, pega_torque, size);
    Serial.println();
    Serial.print("Dados na memoria do torque: ");
    Serial.print(i);
    Serial.println();
    for(int i=0;i<5;i++) {
          Serial.print(pega_torque[i]);
          Serial.print(" ");
    
    }
    Serial.println();
    Serial.print("Dados na memoria do tempo: ");
    Serial.print(i);
    Serial.println();
    for(int i=0;i<5;i++) {
          Serial.print(pega_tempo[i]);
          Serial.print(" ");
    }
  }
  delay(10000);
  saved->push_resistivo(tempo_push, torque_push, size);


}