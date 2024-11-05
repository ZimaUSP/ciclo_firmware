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

  String path = server.uri(); //pega a path

  String string_id = server.arg("id"); //pega id da session em string

  Serial.print("URL: ");
  Serial.println(string_id);
  
  int id = string_id.toInt(); //transforma o id string para int 

  int size = 5;
  double dados_torque[size];
  int dados_tempo[size];

  if(path == "/api/resistivo/sessions"){ // requisição dos dados do modo resistivo
    saved->get_resistivo(id, dados_tempo, dados_torque, size);
  }
  else if(path == "/api/passivo/sessions"){ // requisição dos dados do modo passivo
    saved->get_passivo(id, dados_tempo, dados_torque, size);
  }
  else if(path == "/api/normal/sessions"){ // requisição dos dados do modo normal
    saved->get_normal(id, dados_tempo, dados_torque, size);
  }

  JsonDocument doc; //cria objeto json

  JsonArray tempo = doc["tempo"].to<JsonArray>(); //cria o objeto tempo no documento json

  for(int i=0; i<size; i++){
    tempo.add(dados_tempo[i]); //add os valores colhidos para o objeto tempo
  }

  JsonArray torque = doc["torque"].to<JsonArray>(); //cria o objeto torque no documento json

  for(int i=0; i<size; i++){
    torque.add(dados_torque[i]); //add os valores colhidos para o objeto torque
  }

  String output;

  doc.shrinkToFit();  // optional

  serializeJson(doc, output); //serializa o objeto (formata ele para string)

  server.send(200, "text/json", output); //envia output em formato json
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

  //endpoints
  server.on("/api/resistivo/sessions", handleAPI); //pega dados resistivo
  server.on("/api/passivo/sessions", handleAPI); //pega dados passivo
  server.on("/api/normal/sessions", handleAPI); //pega dados normal

  server.begin();
}

void loop() {
  server.handleClient();
}