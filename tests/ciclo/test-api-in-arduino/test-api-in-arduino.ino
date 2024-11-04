#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "config.hpp"
#include "Memory.hpp"

WebServer server(80);  // definir porta do servidor

// definindo credenciais
const char* ssid = "Zima";
const char* password = "enzimasUSP";

Memory *saved;

void handleAPI() {
  // send response to request
  // server.send(int STATUS, string CONTENT-TYPE, string DATA_TO_SEND);
  Serial.print("URL: ");
  Serial.println(server.arg("id"));
  int size = 5;
  double dados_torque[size];
  int dados_tempo[size];

  saved->get_resistivo(1, dados_tempo, dados_torque, size);

  JsonDocument doc;

  JsonArray tempo = doc["tempo"].to<JsonArray>();

  for(int i=0; i<size; i++){
    tempo.add(dados_tempo[i]);
  }

  JsonArray torque = doc["torque"].to<JsonArray>();

  for(int i=0; i<size; i++){
    torque.add(dados_torque[i]);
  }

  String output;

  doc.shrinkToFit();  // optional

  serializeJson(doc, output);

  server.send(200, "text/json", output);
}

void setup() {
  
  int sessions = 8;
  const char* name_spc = "resistivo";

  saved = new Memory(name_spc, sessions);//a string aqui eh o namespace

  // iniciar conexão
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  // iniciando comunicação serial
  Serial.begin(9600);
  
  // aguardar conexão wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/plain", "hello");
  });

  server.on("/api/resistivo/sessions", handleAPI);

  server.begin();
}

void loop() {
  server.handleClient();
}