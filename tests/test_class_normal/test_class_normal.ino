#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "current_sensor.hpp"
#include "PID.hpp"
#include "config.hpp"
#include "Button.hpp"
#include "Joystick.hpp"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>
#include<normal.hpp>


#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <ArduinoJson.h>

#include "soc/sens_reg.h" 
#include "CSV.hpp"
#include "Memory.hpp"
#include "Website.hpp"

//******CONSTRUCTOR******//

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD lib
SimpleTimer lcd_timer;
SimpleTimer rpmTime;
SimpleTimer torque_Time;
TaskHandle_t TaskWifiHandle;

//******OBJECTS******//
Encoder* encoder;
H_bridge_controller* motorController;
Button* btn;
PID* PID_vel;
Joystick* joy;
CSV *csv;
Memory* saved;
WEBSITE* web;
current_sensor *cur;

//******GLOBAL VARIABELS******//
//Numero de sessoes website/memoria
#define MAX_SAMPLES 5
#define N_SESSIONS 8

// Time
unsigned long current_t;
unsigned long last_t;
unsigned long delta_t;
//int t_Duration;
//char t[2];

// Control
double current_pulses;
double last_pulses;
double delta_pulses;
double delta_ciclos;
float actual_rpm;
int goal_rpm;
//int verif;
int output;
char STATE = MODE;
double revolutions;
bool joystick_check;

//resistivo
double t0, torque, torque_max, torque_min, torque_med;
float acs;
char t[8192];  // Aumentado o tamanho do buffer de sprintf
int offset, pot, sum, i, contador,pwm_motor;
//double  t_Duration = 0.5;
int verif = 1;
int n_sessions;
double lista_values [MAX_SAMPLES];
int tempo [MAX_SAMPLES];

//Parametros para joystick
uint32_t adc_register;
uint32_t wifi_register;

//Parametros para website
WebServer server(80);  
const char* ssid = "Zima";     // Substitua pelo nome da sua rede Wi-Fi
const char* password = "enzimasUSP"; // Substitua pela senha da rede
const char* MDNSDOMAIN = "ciclo";

void setup(){
    Serial.begin(9600);
    inicializaComponentes();
    delay(500); 
    
    motorController->Set_L(0);
    lcd_timer.setInterval(duration() * 60000);
    delay(500);
    lcd.clear();

    normal* n = new normal(contador, tempo, lista_values, lcd, rpmTime, lcd_timer, 
                          encoder, motorController, saved, sample_t);
    n->executaNormal();
    delay(10000);
    reset();
}


void loop(){
  
}



void inicializaComponentes() {
  // Inicialização dos objetos
  encoder = new Encoder(a_pin, b_pin, 0, Nominal_pulses, Mode);
  encoder->init();

  motorController = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
  motorController->init();

  PID_vel = new PID(1.4, 0.008, kd, i_saturation);


  // Inicialização das variáveis
  last_t = millis();
  last_pulses = encoder->getPulses();

  btn = new Button(btn_pin, 2);
  btn->init();

  lcd.init();  // initialize the lcd
  //lcd.noBacklight();
  last_t = millis();

  joy = new Joystick(pot_pin, adc_register, wifi_register);
  cur = new current_sensor(acs_pin, 18, 20);
  cur->init();
  csv = new CSV();
  web = new WEBSITE();
  saved = new Memory(N_SESSIONS);
    
}

/*
void TaskWifiCode( void * pvParameters ){
  Serial.print("TaskWifi running on core ");
  Serial.println(xPortGetCoreID());

  conectarWiFi();

  while (true) {
    server.handleClient();
    delay(2);//allow the cpu to switch to other tasks
  }
}
*/
int duration() {
  int t_Duration = 0 ;
  lcd.setCursor(0, 0);
  lcd.print("              ");
  while (!btn->getPress()){
         if (joy->middle())
            {
                joystick_check=true;
            }
            if (joy->left() && joystick_check)
            {
                if(t_Duration !=0 ){
                    t_Duration--;
                    joystick_check = false;
                }
            }
            //else if (analogRead(pot_pin) == 4095 && joystick_check)
            else if (joy->right() && joystick_check)
            {
                if(t_Duration != 10){
                    t_Duration++;
                    joystick_check = false;
                }
            }   
    lcd.setCursor(0, 0);  // MAX(15,1) linha, coluna
    lcd.print("Duration: ");
    sprintf(t, "%02d", t_Duration);
    lcd.print(t);
    lcd.print(":00  ");
  }
  return t_Duration;
}
/*
void conectarWiFi() {
    Serial.println("Conectando ao Wi-Fi...");
	  adc_register = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG); // Wifi with ADC2 on ESP32 workaround.
	  WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
	  wifi_register = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);
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

    server.on("/inline", []() {
      server.send(200, "text/plain", "this works as well");
    });

    setEndpoints();

    if (!MDNS.begin(MDNSDOMAIN)) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");

    server.begin();

    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
}
*/
void reset() {
  ESP.restart();
}

void handleRoot() {
  server.send(200, "text/html", "<html><head><title>ESPSERVER</title></head><body><h1>hello from esp32!</h1></body></html>");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setEndpoints() {
  
  server.on("/chart", [](){
    String Websitehtml = web->websiteChart();
    server.send(200,"text/html", Websitehtml);
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

  server.on("/data/resistivo/sessions", getData); //pega dados resistivo
  // server.on("/data/passivo/sessions", getData); //pega dados passivo
  server.on("/data/normal/sessions", getData); //pega dados normal

  server.on("/number/resistivo/sessions", numberSessions); //pega o número de sessões 
  server.on("/number/passivo/sessions", numberSessions); //pega o número de sessões 
  server.on("/number/normal/sessions", numberSessions); //pega o número de sessões 

  server.on("/data/resistivo/sessions/csv", getCSV);
  server.on("/data/passivo/sessions/csv", getCSV);
  server.on("/data/normal/sessions/csv", getCSV);
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