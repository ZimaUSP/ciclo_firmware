#include "config.hpp"
#include "Memory.hpp"
#include <WiFi.h> 
#include <WiFiClient.h>
#include <WebServer.h>

Memory *saved;

int size = 5;
int sessions = 8;

WebServer server(80); 

const char* ssid = "Zima";     // Substitua pelo nome da sua rede Wi-Fi
const char* password = "enzimasUSP"; // Substitua pela senha da rede

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

  server.begin();
  
}

void loop() {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}