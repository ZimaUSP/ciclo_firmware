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
#include <iostream>
#include <algorithm>  // Para std::max_element e std::min_element
#include <numeric>    // Para std::accumulate

#include <WiFiClient.h>
#include <WebServer.h>

#include <ArduinoJson.h>

#include "soc/sens_reg.h" // needed for manipulating ADC2 control register
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
double pageSelec;
double revolutions;
bool joystick_check;

//resistivo
double t0, torque, torque_max, torque_min, torque_med;
float acs;
char t[10];  // Aumentado o tamanho do buffer de sprintf
int offset, pot, sum, i, contador,pwm_motor;
double  t_Duration = 0.5;
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

//******FUNCTIONS******//

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

    server.begin();
}

void TaskWifiCode( void * pvParameters ){
  Serial.print("TaskWifi running on core ");
  Serial.println(xPortGetCoreID());

  conectarWiFi();

  while (true) {
    server.handleClient();
    delay(2);//allow the cpu to switch to other tasks
  }
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
  last_t = millis();

  joy = new Joystick(pot_pin, adc_register, wifi_register);
  cur = new current_sensor(acs_pin, 18, 20);
  cur->init();
  csv = new CSV();
  web = new WEBSITE();
  saved = new Memory(N_SESSIONS);
    
}

// Get frequecy of pot (incluir na classe do potenciometro)
int mapPotValueToRPM(int potValue) {
  return map(potValue, 0, 4095, 0, 50);  // rpm
}

// Control to evit Overflow
void resetEncoderIfExceedsLimit() {
  if (current_pulses > MAX_ENCODER_VALUE || current_pulses < -MAX_ENCODER_VALUE) {
    encoder->setPulses(0);
    last_pulses = 0;
  }
}

// PID control
void controlMotorSpeedWithPID() {
  output = PID_vel->computePID(actual_rpm, goal_rpm, tolerance);
  
  Serial.print("goal: ");
  Serial.print(goal_rpm);
  Serial.print("; actual rpm: ");
  Serial.print(actual_rpm);
  Serial.print("; tolerance: ");
  Serial.print(tolerance);
  Serial.print("; output: ");
  Serial.println(output);
  
  if (output < 0) {
    Serial.println("going left");
    output = max(output, -MAX_PWM);
    motorController->Set_L(-output);
  } else {
    Serial.println("going right");
    output = min(output, MAX_PWM);
    motorController->Set_R(output);
  }
}

// Turns it off and on to reset the program

void reset() {
  ESP.restart();
}
// Implemetation passive Mode (Criar Classe passivo para implementar esses controle e evitar de ter muita coisa na main)
void passivo() {
  contador = 0;
  lcd_timer.reset();
  rpmTime.reset();
  // Resetar o array de torques
  for (int i = 0; i < MAX_SAMPLES; i++) {
      lista_values[i] = 3.0;
      tempo[i]=3;
  }

  while (!lcd_timer.isReady()) {  //tempo nao acaba
    server.handleClient(); 

    if (rpmTime.getTimePassed() > sample_t) {

      current_pulses = encoder->getPulses();
      delta_pulses = current_pulses - last_pulses;
      actual_rpm = delta_pulses * 1.01;

      if(contador < MAX_SAMPLES) {
        lista_values[contador] = actual_rpm;
        tempo[contador]=contador*sample_t;
        contador++;
      }
      Serial.print(actual_rpm);
      Serial.print("; time: ");
      Serial.println(timePassed);
      */

      resetEncoderIfExceedsLimit();
      controlMotorSpeedWithPID();

      rpmTime.reset();
      last_pulses = current_pulses;
    }
    
    printTime();
  }
  saved->push_passivo(tempo, lista_values, MAX_SAMPLES);
}

/*void website_data(){
    
    Serial.print("IP local: ");
    Serial.println(WiFi.localIP());
    server.on("/data/csv/resistivo/sessions", []() { //mudar string de acordo com o modo
      for(int i=0; i<N_SESSIONS; i++) {
        double data_torque [MAX_SAMPLES];
        int data_tempo [MAX_SAMPLES];
        saved->get_resistivo(i, tempo, lista_values);
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

}*/

/*void getData() {
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
    saved->get_resistivo(id, tempo, lista_values);
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
}*/

//Implementation resistivo mode
void executarLogicaResistivo() {

    //if (done)
    //  return;'
    Serial.println("executar lógica");
    //website_data();
    pwm_motor = def_pwm_motor();

    delay(500);
    lcd_timer.setInterval(duration() * 60000);
    delay(500);
    lcd.clear();


    if (verification() < 1) {
        delay(500);
        return;
    }


    resistivo();
    print_torque_results();
    delay(500);
    //website_data(); // AQUI NAO FUNCIONA
    //endpoints(); // AQUI NAO FUNCIONA
    Serial.println("logica executada");
    //done = true;
}

int def_pwm_motor() {
    int pwm_motor = 50;  // Inicialize o valor do PWM
    lcd.setCursor(0, 0);
    lcd.print("              ");
    
    while (!btn->getPress()) { 
        Serial.println(joy->get_power());
        //Serial.println("Botão não apertado");
        server.handleClient();
        if (joy->middle()) {
            Serial.println("Middle");
            joystick_check = true;
        }
        if (joy->left() && joystick_check) {
            Serial.println("Left");
            if (pwm_motor >= 10) {
                pwm_motor -= 10;
                joystick_check = false;
            }
        } else if (joy->right()  && joystick_check) {
            Serial.println("Right");
            if (pwm_motor < 100) {
                pwm_motor += 10;
                joystick_check = false;
            }
        }
        lcd.setCursor(1, 0);
        lcd.print("PWM: ");
        sprintf(t, "%02d", pwm_motor);
        lcd.print(t);
        delay(10);  // Evitar sobrecarga do loop
    }
    return pwm_motor;
}

void resistivo() {
  delay(200);
  contador = 0;
    // Resetar o array de torques
    for (int i = 0; i < MAX_SAMPLES; i++) {
        lista_values[i] = 3.0;
        tempo[i]=3;
    }
    lcd_timer.reset();
    motorController->Set_L(pwm_motor);

    while (!lcd_timer.isReady() ) {
      server.handleClient(); 
        if (torque_Time.getTimePassed() > sample_t * 50) {
            if (contador < MAX_SAMPLES) {
                //Serial.println("ok");
                acs = cur->get_current();
                torque = cur->get_torque(acs);
                //Serial.print(torque);
                //Serial.print(", ");
                //Serial.println(lcd_timer.getTimePassed());
                lista_values[contador] = torque;
                tempo[contador]=contador*sample_t * 50;
                contador++;
                torque_Time.reset();
                lcd.setCursor(1, 1);
                lcd.print("TORQUE: ");
                sprintf(t, "%02f", torque);
                lcd.print(t);
            }
        }
        printTime();
    }

    saved->push_resistivo(tempo, lista_values, MAX_SAMPLES);
    delay(500);
    if (contador > 0) {  // Evitar divisão por zero
        torque_max = lista_values[0];
        torque_min = lista_values[0];
        double soma = 0.0;

        for (int i = 0; i < contador; i++) {
            if (lista_values[i] > torque_max) {
                torque_max = lista_values[i];
            }
            if (lista_values[i] < torque_min) {
                torque_min = lista_values[i];
            }
            soma += lista_values[i];
        }
        // Calcular a média
        torque_med = soma / contador;
    } else {
        torque_max = 0;
        torque_min = 0;
        torque_med = 0;
    }

    delay(100);
    motorController->Set_L(0);
}

void print_torque_results() {
    lcd.clear();
    delay(10);
    lcd.setCursor(0, 0);
    lcd.print("Max:");
    sprintf(t, "%.2f", torque_max);  // Exibe com 2 casas decimais
    lcd.print(t);
    lcd.print("Min:");
    sprintf(t, "%.2f", torque_min);
    lcd.print(t);
    lcd.setCursor(0, 1);
    lcd.print("Med:");
    sprintf(t, "%.2f", torque_med);
    lcd.print(t);
    while (!btn->getPress()) {
        server.handleClient();
        delay(10);  // Evitar loop sem pausa
    }
    lcd.clear();
    delay(10);
    lcd.setCursor(0, 0);
    lcd.print("Fim");
    delay(1000);
    lcd.clear();
}

// Implemetation normal Mode (Criar Classe normal para implementar esses controle e evitar de ter muita coisa na main)

void normal() {
  contador = 0;
  lcd_timer.reset();
  rpmTime.reset();
  //website_data();
  while (!lcd_timer.isReady()) {  //tempo nao acaba
    server.handleClient(); 

    if (rpmTime.getTimePassed() > 400) {
      current_pulses = encoder->getPulses();
      delta_pulses = current_pulses - last_pulses;
      double revolutions = delta_pulses/pulses_per_rev;
      actual_rpm = revolutions*(60000/400);

      if(contador < MAX_SAMPLES) {
        lista_values[contador] = torque;
        tempo[contador]=contador*sample_t;
        contador++;
      }
      
      resetEncoderIfExceedsLimit();
      rpmTime.reset();
      last_pulses = current_pulses;
      Serial.print("; actual rpm: ");
      Serial.print(actual_rpm);
      Serial.print("; time: ");
      Serial.println(lcd_timer.getTimePassed());

    }
    lcd.setCursor(0, 1);
    lcd.print("Frequency: ");
    lcd.print(actual_rpm);
    printTime();
  }
  saved->push_normal(tempo, lista_values, MAX_SAMPLES);
}

// Select funcition
void selectFunction() {
  if (pageSelec ==0 ) {
    lcd.print("Escolha o modo ");
    lcd.setCursor(0, 1);
    lcd.print("Modo: Normal   ");
    lcd.setCursor(0, 0);
    STATE = NORMAL;
  } else if (pageSelec == 1) {
    lcd.print("Escolha o modo ");
    lcd.setCursor(0, 1);
    lcd.print("Modo: Passive   ");
    lcd.setCursor(0, 0);
    STATE = PASSIVE;
  }
  else if (pageSelec == 2) {
    lcd.print("Escolha o modo ");
    lcd.setCursor(0, 1);
    lcd.print("Modo: Resistivo   ");
    lcd.setCursor(0, 0);
    STATE = PASSIVE;
  }
  
}

// Print in LCD the MODE select
void printMode() {
  lcd.setCursor(0, 0);
  switch (STATE) {
    case NORMAL:
      lcd.print("Selecionado:  ");
      lcd.setCursor(0, 1);
      lcd.print("Modo Normal   ");
      lcd.setCursor(0, 1);
      return;

    case PASSIVE:
      lcd.print("Selecionado:  ");
      lcd.setCursor(0, 1);
      lcd.print("Modo Passivo  ");
      lcd.setCursor(0, 1);
      return;
    case FADE:
      lcd.print("Selecionado:  ");
      lcd.setCursor(0, 1);
      lcd.print("Modo Resistivo     ");
      lcd.setCursor(0, 1);
      return;
  }
}


// Select Duration
int duration() {
  int t_Duration;
  lcd.setCursor(0, 0);
  lcd.print("              ");
  while (!btn->getPress()){
    server.handleClient();
         //if (analogRead(pot_pin) != 0 && analogRead(pot_pin) != 4095)
         if (joy->middle())
            {
                joystick_check=true;
            }
            //if (analogRead(pot_pin) == 0 && joystick_check)
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

// Print stopwatch in LCD

void printTime() {
  lcd.setCursor(0, 0);
  lcd.print("Duration: ");
  sprintf(t, "%02d", lcd_timer.getMinutes());
  lcd.print(t);
  lcd.print(":");
  sprintf(t, "%02d", lcd_timer.getSeconds());
  lcd.print(t);
}

void printFrequency() {
  lcd.setCursor(0, 1);
  lcd.print("Frequency: ");
  sprintf(t, "%02d", goal_rpm);
  lcd.print(t);
}


// Get frequency (Implementar como Vetor quando criar a Classe)
int goalRPM() {

   while (!btn->getPress()){
         //if (analogRead(pot_pin) != 0 && analogRead(pot_pin) != 4095)
         if (!joy->right() && !joy->left())
            {
                joystick_check=true;
            }
            //if (analogRead(pot_pin) == 0 && joystick_check)
            if (joy->left() && joystick_check)
            {
                if(goal_rpm !=0 ){
                    goal_rpm--;
                    joystick_check = false;
                }
            }
            //else if (analogRead(pot_pin) == 4095 && joystick_check)
            else if (joy->right() && joystick_check)
            {
                if(goal_rpm != 50){
                    goal_rpm++;
                    joystick_check = false;
                }
            }   
    printFrequency();
   }
  return goal_rpm;
}


// Verification

int verification() {
  lcd.clear();
  while (!btn->getPress()) {
    server.handleClient();
    //if (analogRead(pot_pin)==0)
    if (joy->left())
    {
      verif = 0;
    }
    //if (analogRead(pot_pin)==4095)
    if (joy->right())
    {
      verif = 1;
    }
    lcd.setCursor(0, 0);
    lcd.print("Freq: ");
    sprintf(t, "%02d", goal_rpm);
    lcd.print(t);
    lcd.print("|");
    lcd.print("Time:");
    sprintf(t, "%02d", lcd_timer.getInterval() / 60000);
    lcd.print(t);
    lcd.setCursor(0, 1);
    if (verif == 0) {
      lcd.print("Nao            ");
      lcd.setCursor(0, 1);
      lcd.noBacklight();
    }
    if (verif == 1) {
      lcd.print("Sim            ");
      lcd.setCursor(0, 1);
      lcd.noBacklight();
    }
  }
  lcd.clear();
  return verif;
}


//******MAIN******//

void setup() {
  Serial.begin(9600);
  inicializaComponentes();
  char* name_spc = "resistivo";

<<<<<<< HEAD
  Serial.begin(9600);
  // Conectar ao Wi-Fi na inicialização
  //conectarWiFi();
  //endpoints();
  xTaskCreatePinnedToCore(
                      TaskWifiCode,   // Task function.
                      "TaskWIfi",     // name of task.
                      10000,       // Stack size of task
                      NULL,        // parameter of the task
                      1,           // priority of the task
                      &TaskWifiHandle,      // Task handle to keep track of created task
                      0);          // pin task to core 0
  delay(500); 
  STATE = STAND_BY;
  
}

void loop() {

  switch (STATE) {
    case PASSIVE:
      //website_data();
      goal_rpm = goalRPM();
      delay(500);
      lcd_timer.setInterval(duration() * 60000);
      delay(500);
      lcd.clear();
      verif = verification();
      delay(500);
      if (verif >= 1) {
        delay(500);
      }
      if (verif < 1) {
        STATE = STAND_BY;
        delay(500);
        return;
      }
      passivo();
      motorController->Set_L(0);
      STATE = STAND_BY;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FIM");
      delay(2000);
      reset();
      return;

    case STAND_BY:
       while (!btn->getPress()){
          //if (analogRead(pot_pin) != 0 && analogRead(pot_pin) != 4095)
          if (!joy->right() && !joy->left())
          {
            joystick_check=true;
          }
          
        //if (analogRead(pot_pin) == 0 && joystick_check)
        if (joy->left() && joystick_check)
        {
            if(pageSelec !=0 ){
                pageSelec--;
                joystick_check = false;
            }

        }
        //else if (analogRead(pot_pin) == 4095 && joystick_check)
        else if (joy->right() && joystick_check)
        {
            if(pageSelec != 2){
                pageSelec++;
                joystick_check = false;
            }
        }
        
        selectFunction();
        lcd.noBacklight();
      }
      printMode();
      delay(500);
      return;

    case NORMAL:
      delay(100);
      motorController->Set_L(0);
      lcd_timer.setInterval(duration() * 60000);
      delay(500);
      lcd.clear();

      normal();

      delay(500);
      reset();
      return;

    case FADE: //caso resistivo
      delay(100);
      motorController->Set_R(0);
      executarLogicaResistivo();
      server.handleClient();
      delay(2);
      return;
  
  }




}
