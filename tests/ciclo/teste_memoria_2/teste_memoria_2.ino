#include "config.hpp"
#include "Memory.hpp"
#include <WiFi.h> 
#include <WiFiClient.h>
#include <WebServer.h>
#include <nvs_flash.h>

Memory *saved;

int size = 5;
int size2 = 4;
int size3 = 3;
int sessions = 8;

WebServer server(80); 

const char* ssid = "Panda";     // Substitua pelo nome da sua rede Wi-Fi
const char* password = "NekoForlifE690"; // Substitua pela senha da rede

void numberSessions() {
  Serial.println("getting num sessions...");
  //saved = new Memory();//a string aqui eh o namespace
  int num = saved->get_saved_sessions_resistivo();
  Serial.println(num);
  String out = "num session: " + String(num);
  server.send(200, "text/plain", out);
}


void setup() {
  Serial.begin(9600);
  nvs_flash_erase(); // erase the NVS partition and...
  nvs_flash_init(); // initialize the NVS partition.
  /*
  int tempoget[size + 1] = {};
  double valget[size + 1] = {};

  for(int n = 0; n < 3; n++){
    saved->get_resistivo(n, tempoget, valget);
    Serial.println("Resistivo");
    Serial.println("Dados na memoria do tempo: ");
    //for(int i = 0; i < tempoget[0]; i++){
    for(int i = 0; i < size; i++){
      Serial.print(tempoget[i]);
      Serial.print(", ");
    }

    Serial.println(" ");

    Serial.println("Dados na memoria do torque: ");
    //for(int i = 0; i < valget[0]; i++){
    for(int i = 0; i < size; i++){
      Serial.print(valget[i]);
      Serial.print(", ");
    }
    Serial.println(" ");
  }*/
  //while(true);
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", numberSessions);
  Serial.println("getting num sessions...");
  saved = new Memory();//a string aqui eh o namespace
  int num = saved->get_saved_sessions_resistivo();
  int tempo[size] = {10,20,30,40,50};
  int tempo2[size] = {11,21,31,41,51};
  int tempo3[size] = {12,22,32,42,52};

  double val[size] = {15,25,35,45,55};
  double val2[size] = {16,26,36,46,56};
  double val3[size] = {17,27,37,47,57};
  //int tempoget[size + 1] = {};
  //double valget[size + 1] = {};

  saved->push_resistivo(tempo,val,size);
  saved->push_resistivo(tempo2,val2,size);
  saved->push_resistivo(tempo3,val3,size);

  int tempo2[size] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
                    21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,
                    36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};

  double val4[size] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
                    21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,
                    36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};

  double val5[size] = {50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,
                     30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,
                     10,9,8,7,6,5,4,3,2,1};

  double val6[size] = {50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,
                     50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,
                     50,50,50,50,50,50,50,50,50,50}

  saved->push_normal(tempo2,val4,50);
  saved->push_normal(tempo2,val5,50);
  saved->push_normal(tempo2,val6,50);
  
  for(int n = 0; n < 3; n++){
    saved->get_resistivo(n, tempoget, valget);
    Serial.println("Resistivo");
    Serial.println("Dados na memoria do tempo: ");
    //for(int i = 0; i < tempoget[0]; i++){
    for(int i = 0; i < size; i++){
      Serial.print(tempoget[i]);
      Serial.print(", ");
    }

    Serial.println(" ");

    Serial.println("Dados na memoria do torque: ");
    //for(int i = 0; i < valget[0]; i++){
    for(int i = 0; i < size; i++){
      Serial.print(valget[i]);
      Serial.print(", ");
    }
    Serial.println(" ");
  }

  for(int n = 0; n < 3; n++){
    saved->get_normal(n, tempoget, valget);
    Serial.println("Normal");
    Serial.println("Dados na memoria do tempo: ");
    //for(int i = 0; i < tempoget[0]; i++){
    for(int i = 0; i < size; i++){
      Serial.print(tempoget[i]);
      Serial.print(", ");
    }

    Serial.println(" ");

    Serial.println("Dados na memoria do torque: ");
    //for(int i = 0; i < valget[0]; i++){
    for(int i = 0; i < size; i++){
      Serial.print(valget[i]);
      Serial.print(", ");
    }
    Serial.println(" ");
    /*String next = saved->printalgo(n);
    Serial.print("a: ");
    Serial.println(next);*/
  }

  //saved->push(tempo,val,size);
  //Serial.print("next: ");
  //Serial.println(next);
  //saved->push(tempo,val,size);
  //Serial.print("next: ");
  //Serial.println(next);
  //saved->push(tempo,val,size);
  //Serial.print("next: ");
  //Serial.println(next);
  //saved->push(tempo,val,size);
  //next = saved->printalgo();
  //Serial.print("next: ");
  //Serial.println(next);
  //saved->push(tempo2,val2,size);
  //next = saved->printalgo();
  //Serial.println(next);
  //saved->push(tempo3,val3,size);
  //saved->push(tempo2,val2,size);
  //saved->push(tempo,val,size);

  Serial.println(num);
  String out = "num session: " + String(num);
  server.send(200, "text/plain", out);
  server.begin();
  
}

void loop() {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}