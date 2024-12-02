#include "config.hpp"
#include <WiFi.h> 
#include <WiFiClient.h>
#include <WebServer.h>
#include "Memory.hpp"
#include "Website.hpp"
#include "CSV.hpp"
#include <ArduinoJson.h>

Memory* database;
CSV *csv;
Memory* saved;
WEBSITE* web;
WebServer server(80); 

#define MAX_SAMPLES 5
#define N_SESSIONS 6

const char* ssid = "Zima";     // Substitua pelo nome da sua rede Wi-Fi
const char* password = "enzimasUSP"; // Substitua pela senha da rede

void setup() {
  Serial.begin(9600);
  saved = new Memory(N_SESSIONS);//a string aqui eh o namespace
  conectarWiFi();
  
  // put your setup code here, to run once:

}

void handleRoot() {
  server.send(200, "text/plain", "hello from esp32!");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}


void conectarWiFi() {
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

    server.on("/", handleRoot);

    setEndpoints();
    
    server.begin();


}

/*
void website_data(){
    Serial.print("IP local: ");
    Serial.println(WiFi.localIP());
    server.on("/dados", []() {
      for(int i=0; i<N_SESSIONS; i++) {
        double data_torque [MAX_SAMPLES];
        int data_tempo [MAX_SAMPLES];
        database->get_resistivo(i, tempo, lista_torque);
        //String data = csv->to_csv("Torque", data_torque, "Tempo", data_tempo, MAX_SAMPLES); //necessario mudar para armazenar cada numero de sessao diferente
        //server.send(200, "text/csv", data); // Envia a página HTML ao navegador
      }
  });


    server.on("/Resistivo/sessions", [](){

      String Websitehtml = web->websiteResistivo();
 
      server.send(200,"text/html", Websitehtml);
    });

    server.on("/Normal/sessions", [](){

      String Websitehtml = web->websiteNormal();
 
      server.send(200,"text/html", Websitehtml);
    });

    server.on("/Passivo/sessions", [](){

      String Websitehtml = web->websitePassivo();
 
      server.send(200,"text/html", Websitehtml);
    });
    


    server.begin();
    Serial.println("HTTP server started");

}
*/

void setEndpoints() {
    server.on("/Resistivo/sessions", [](){
      String Websitehtml = web->websiteResistivo();
      server.send(200,"text/html", Websitehtml);
    });

    server.on("/Normal/sessions", [](){
      String Websitehtml = web->websiteNormal();
      server.send(200,"text/html", Websitehtml);
    });

    server.on("/Passivo/sessions", [](){
      String Websitehtml = web->websitePassivo();
      server.send(200,"text/html", Websitehtml);
    });

  //server.on("/data/resistivo/sessions", getData); //pega dados resistivo
  //server.on("/data/passivo/sessions", getData); //pega dados passivo
  //server.on("/data/normal/sessions", getData); //pega dados normal

  server.on("/number/resistivo/sessions", numberSessions); //pega o número de sessões 
  server.on("/number/passivo/sessions", numberSessions); //pega o número de sessões 
  server.on("/number/normal/sessions", numberSessions); //pega o número de sessões 
}


void getData() {
  // send response to request
  // server.send(int STATUS, string CONTENT-TYPE, string DATA_TO_SEND);

  String path = server.uri(); //pega a path

  String string_id = server.arg("id"); //pega id da session em string

  Serial.print("URL: ");
  Serial.println(string_id);
  
  int id = string_id.toInt(); //transforma o id string para int 

  int size = MAX_SAMPLES;
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

void numberSessions(){

  int n_sessions;

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
  //n_sessions = saved->get_saved_sessions_resistivo();
  JsonDocument doc; // cria o documento em formato json 

  JsonArray sessions = doc["sessions"].to<JsonArray>(); // cria o objeto sessions
  sessions.add(n_sessions); // adiciona valor de sessions para o objeto json sessions

  String output; // cria uma string chamada output

  doc.shrinkToFit();  // optional

  serializeJson(doc, output); //serializa o objeto (formata ele para string)
  server.send(200, "text/json", output);
}