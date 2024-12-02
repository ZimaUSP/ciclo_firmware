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

int n_sessions;

Memory *saved;

void numberSessions(){

  String path = server.uri(); //pega a path

  if(path == "/number/resistivo/sessions"){ // requisição dos dados do modo resistivo
    n_sessions = saved->get_saved_sessions_resistivo();
  }
  else if(path == "/number/passivo/sessions"){ // requisição dos dados do modo passivo
    n_sessions = saved->get_saved_sessions_passivo();
  }
  else if(path == "/number/normal/sessions"){ // requisição dos dados do modo normal
    n_sessions = saved->get_saved_sessions_normal();
  }

  //sessions = saved->get_saved_sessions_resistivo();
  JsonDocument doc; // cria o documento em formato json 
  
  JsonArray sessions = doc["sessions"].to<JsonArray>(); // cria o objeto sessions
  sessions.add(n_sessions); // adiciona valor  de sessions para o objeto json sessions

  String output; // cria uma string chamada output

  doc.shrinkToFit();  // optional

  serializeJson(doc, output); //serializa o objeto (formata ele para string)

  server.send(200, "text/json", output);
}

void getData() {
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

  if(path == "/data/resistivo/sessions"){ // requisição dos dados do modo resistivo
    saved->get_resistivo(id, dados_tempo, dados_torque);
  }
  else if(path == "/data/passivo/sessions"){ // requisição dos dados do modo passivo
    saved->get_passivo(id, dados_tempo, dados_torque);
  }
  else if(path == "/data/normal/sessions"){ // requisição dos dados do modo normal
    saved->get_normal(id, dados_tempo, dados_torque);
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
  
  const char* name_spc = "resistivo";

  int max_sessions = 100;
  saved = new Memory(max_sessions);//a string aqui eh o namespace

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
  server.on("/data/resistivo/sessions", getData); //pega dados resistivo
  server.on("/data/passivo/sessions", getData); //pega dados passivo
  server.on("/data/normal/sessions", getData); //pega dados normal

  server.on("/number/resistivo/sessions", numberSessions); //pega o número de sessões 
  server.on("/number/passivo/sessions", numberSessions); //pega o número de sessões 
  server.on("/number/normal/sessions", numberSessions); //pega o número de sessões 

  server.begin();

  int data_tempo[5] = {0,1,2,3,4};
  double data_torque[5] = {2,4,6,8,10.6};
  saved->push_resistivo(data_tempo, data_torque, 5);
}

void loop() {
  server.handleClient();
}