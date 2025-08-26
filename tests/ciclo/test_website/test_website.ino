#include "config.hpp"
#include <WiFi.h> 
#include <WiFiClient.h>
#include <WebServer.h>
#include "Memory.hpp"
#include "Website.hpp"
#include "CSV.hpp"
#include <ArduinoJson.h>
#include <nvs_flash.h>

TaskHandle_t TaskWifiHandle;

Memory* saved;
WEBSITE* web;
WebServer server(80); 

#define MAX_SAMPLES 5
#define N_SESSIONS 6

#define led 2

const char* ssid = "Zima_5G";     // Substitua pelo nome da sua rede Wi-Fi
const char* password = "enzimasUSP"; // Substitua pela senha da rede

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);

  Serial.print("Main Task running on core ");
  Serial.println(xPortGetCoreID());

  xTaskCreatePinnedToCore(
                    TaskWifiCode,   /* Task function. */
                    "TaskWIfi",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &TaskWifiHandle,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */
    delay(500); 

}

void TaskWifiCode( void * pvParameters ){
  Serial.print("TaskWifi running on core ");
  Serial.println(xPortGetCoreID());

  saved = new Memory();//a string aqui eh o namespace

  // TESTE

  nvs_flash_erase(); // erase the NVS partition and...
  nvs_flash_init(); // initialize the NVS partition.

  int size = 50;
  int size1 = 5;
  int size2 = 4;
  int size3 = 3;
  int sessions = 8;

  server.on("/", numberSessions);
  Serial.println("getting num sessions...");
  saved = new Memory();//a string aqui eh o namespace
  int num = saved->get_saved_sessions_resistivo();
  /*
  int tempo[size1] = {10,20,30,40,50};
  int tempo2[size2] = {11,21,31,41};
  int tempo3[size3] = {12,22,32};

  double val[size1] = {15,25,35,45,55};
  double val2[size2] = {16,26,36,46};
  double val3[size3] = {17,27,37};
  int tempoget[size + 1] = {};
  double valget[size + 1] = {};

  saved->push(tempo,val,size);
  saved->push(tempo2,val2,size2);
  saved->push(tempo3,val3,size3);*/

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
                      50,50,50,50,50,50,50,50,50,50};

    saved->push_normal(tempo2,val4,50);
    saved->push_normal(tempo2,val5,50);
    saved->push_normal(tempo2,val6,50);

  // TESTE

  conectarWiFi();

  while (true) {
    server.handleClient();
    delay(2);//allow the cpu to switch to other tasks
  }
}

void handleRoot() {
  server.send(200, "text/plain", "hello from esp32!");
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  Serial.print("turn on led - ");
  Serial.println(xPortGetCoreID());
  digitalWrite(led, HIGH);
  delay(1000);
  Serial.print("turn off led - ");
  Serial.println(xPortGetCoreID());
  digitalWrite(led, LOW);
  delay(1000);
  */
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

    Serial.println("Setting up endpoints...");
  
    server.on("/chart", [](){
      String Websitehtml = web->websiteChart();
      server.send(200,"text/html", Websitehtml);
    });

    server.on("/graficos", [](){
      String Websitehtml = web->websiteGRAFICOS();
      server.send(200,"text/html", Websitehtml);
    });
      
    /*
    web->websiteTESTE(server,
                      saved->get_saved_sessions_resistivo(),
                      saved->get_saved_sessions_normal(),
                      saved->get_saved_sessions_passivo());
*/
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

  server.on("/data/resistivo/sessions", getData); //pega dados resistivo
  //server.on("/data/passivo/sessions", getData); //pega dados passivo
  //server.on("/data/normal/sessions", getData); //pega dados normal

  server.on("/number/resistivo/sessions", numberSessions); //pega o número de sessões 
  server.on("/number/passivo/sessions", numberSessions); //pega o número de sessões 
  server.on("/number/normal/sessions", numberSessions); //pega o número de sessões 

  server.on("/data/resistivo/sessions/csv", getCSV);
  server.on("/data/passivo/sessions/csv", getCSV);
  server.on("/data/normal/sessions/csv", getCSV);
}

void getCSV() {
  String path = server.uri(); //pega a path

  String string_id = server.arg("id"); //pega id da session em string

  Serial.print("URL: ");
  Serial.println(string_id);
  
  int id = string_id.toInt(); //transforma o id string para int 

  int size = MAX_SAMPLES;
  double dados_torque[size];
  int dados_tempo[size];
  CSV *csv = new CSV();
  String data = "";

  if(path == "/data/resistivo/sessions/csv"){ // requisição dos dados do modo resistivo
    saved->get_resistivo(id, dados_tempo, dados_torque);
    data = csv->to_csv("Torque", dados_torque, "Tempo", dados_tempo, MAX_SAMPLES);
  }
  else if(path == "/data/passivo/sessions/csv"){ // requisição dos dados do modo passivo
    saved->get_passivo(id, dados_tempo, dados_torque);
    data = csv->to_csv("Frequência", dados_torque, "Tempo", dados_tempo, MAX_SAMPLES);
  }
  else if(path == "/data/normal/sessions/csv"){ // requisição dos dados do modo normal
    saved->get_normal(id, dados_tempo, dados_torque);
    data = csv->to_csv("Frequência", dados_torque, "Tempo", dados_tempo, MAX_SAMPLES);
  }
  server.send(200, "text/csv", data);
  
}

void getData() {
  Serial.println("Setting up getData endpoint");

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