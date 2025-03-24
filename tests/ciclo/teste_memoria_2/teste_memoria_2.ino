#include "config.hpp"
#include "Memory.hpp"
#include <WiFi.h> 
#include <WiFiClient.h>
#include <WebServer.h>
#include <nvs_flash.h>

Memory *saved;

int size = 5;
int sessions = 8;

WebServer server(80); 

const char* ssid = "MAIS VIRUS INTEL";     // Substitua pelo nome da sua rede Wi-Fi
const char* password = "14021973"; // Substitua pela senha da rede

void numberSessions() {
  Serial.println("getting num sessions...");
  saved = new Memory(sessions);//a string aqui eh o namespace
  int num = saved->get_saved_sessions_resistivo();
  Serial.println(num);
  String out = "num session: " + String(num);
  server.send(200, "text/plain", out);
}


void setup() {
  Serial.begin(9600);
  //nvs_flash_erase(); // erase the NVS partition and...
  //nvs_flash_init(); // initialize the NVS partition.
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
  saved = new Memory(sessions);//a string aqui eh o namespace
  int num = saved->get_saved_sessions_resistivo();
  int tempo[size] = {10,20,30,40,50};
  int tempo2[size] = {11,21,31,41,51};
  int tempo3[size] = {12,22,32,42,52};

  double val[size] = {15,25,35,45,55};
  double val2[size] = {16,26,36,46,56};
  double val3[size] = {17,27,37,47,57};
  int tempoget[size] = {};
  double valget[size] = {};

  for(int n = 0; n < 1; n++){
    saved->get(n, tempoget, valget);
    for(int i = 0; i < size; i++){
      Serial.println(tempoget[i]);
      Serial.println(valget[i]);
    }
    Serial.println(n);
  }

  int next = saved->printalgo();
  Serial.print("next: ");
  Serial.println(next);
  saved->push(tempo,val,size);
  Serial.print("next: ");
  Serial.println(next);
  saved->push(tempo,val,size);
  Serial.print("next: ");
  Serial.println(next);
  saved->push(tempo,val,size);
  Serial.print("next: ");
  Serial.println(next);
  saved->push(tempo,val,size);
  //next = saved->printalgo();
  //Serial.print("next: ");
  //Serial.println(next);
  //saved->push(tempo2,val2,size);
  //next = saved->printalgo();
  //Serial.println(next);
  //saved->push(tempo3,val3,size);
  //saved->push(tempo2,val2,size);
  //saved->push(tempo,val,size);

  for(int n = 0; n < 9; n++){
    saved->get(n, tempoget, valget);
    for(int i = 0; i < size; i++){
      Serial.println(tempoget[i]);
      Serial.println(valget[i]);
    }

    Serial.println(n);
  }

  Serial.println(num);
  String out = "num session: " + String(num);
  server.send(200, "text/plain", out);
  server.begin();
  
}

void loop() {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}