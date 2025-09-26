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

#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <WiFiManager.h> 

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
// #define MAX_SAMPLES 5
// #define N_SESSIONS 8

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
WiFiManager wm; //objeto da classe wifimanager
const char* MDNSDOMAIN = "ciclo";

//******FUNCTIONS******//

void setEndpoints() {

  server.on("/", [](){
    server.sendHeader("Location", "/resistivo/sessions", true);
    server.send(301, "text/plain", "");
  });
  
  server.on("/chart", [](){
    String Websitehtml = web->websiteChart();
    server.send(200,"text/html", Websitehtml);
  });

  server.on("/resistivo/sessions", [](){
    String Websitehtml = web->websiteResistivo();
    server.send(200,"text/html", Websitehtml);
  });

  server.on("/normal/sessions", [](){
    String Websitehtml = web->websiteNormal();
    server.send(200,"text/html", Websitehtml);
  });

  server.on("/passivo/sessions", [](){
    String Websitehtml = web->websitePassivo();
    server.send(200,"text/html", Websitehtml);
  });

  server.on("/graficos", [](){
    String Websitehtml = web->websiteGRAFICOS();
    server.send(200,"text/html", Websitehtml);
  });

  server.on("/data/resistivo/sessions", getData); //pega dados resistivo
  server.on("/data/passivo/sessions", getData); //pega dados passivo
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

  int size = 0;
  double* dados_torque = NULL;
  int* dados_tempo = NULL;
  CSV *csv = new CSV();
  String data = "";
  if(path == "/data/resistivo/sessions/csv"){ // requisição dos dados do modo resistivo
    size = saved->size_resistivo(id);
    dados_torque = (double*)calloc(size, sizeof(double));
    dados_tempo =  (int*)calloc(size, sizeof(int));
    saved->get_resistivo(id, dados_tempo, dados_torque);
    data = csv->to_csv("Torque", dados_torque, "Tempo", dados_tempo, size);
  }
  else if(path == "/data/passivo/sessions/csv"){ // requisição dos dados do modo passivo
    size = saved->size_passivo(id);
    dados_torque = (double*)calloc(size, sizeof(double));
    dados_tempo =  (int*)calloc(size, sizeof(int));
    saved->get_passivo(id, dados_tempo, dados_torque);
    data = csv->to_csv("Frequência", dados_torque, "Tempo", dados_tempo, size);
  }
  else if(path == "/data/normal/sessions/csv"){ // requisição dos dados do modo normal
    size = saved->size_normal(id);
    dados_torque = (double*)calloc(size, sizeof(double));
    dados_tempo =  (int*)calloc(size, sizeof(int));
    saved->get_normal(id, dados_tempo, dados_torque);
    data = csv->to_csv("Frequência", dados_torque, "Tempo", dados_tempo, size);
  }
  server.send(200, "text/csv", data);
  free(dados_torque);
  free(dados_tempo);
  }

void getData() {
  // send response to request
  // server.send(int STATUS, string CONTENT-TYPE, string DATA_TO_SEND);

  String path = server.uri(); //pega a path

  String string_id = server.arg("id"); //pega id da session em string

  Serial.print("URL: ");
  Serial.println(string_id);

  int id = string_id.toInt(); //transforma o id string para int 

  int size = 0;
  double *dados_torque = NULL;
  int *dados_tempo =  NULL;

  if(path == "/data/resistivo/sessions"){ // requisição dos dados do modo resistivo
    size = saved->size_resistivo(id);
    dados_torque = (double*)calloc(size, sizeof(double));
    dados_tempo =  (int*)calloc(size, sizeof(int));
    saved->get_resistivo(id, dados_tempo, dados_torque);
  }
  else if(path == "/data/passivo/sessions"){ // requisição dos dados do modo passivo
    size = saved->size_passivo(id);
    dados_torque = (double*)calloc(size, sizeof(double));
    dados_tempo =  (int*)calloc(size, sizeof(int));
    saved->get_passivo(id, dados_tempo, dados_torque);
  }
  else if(path == "/data/normal/sessions"){ // requisição dos dados do modo normal
    size = saved->size_normal(id);
    dados_torque = (double*)calloc(size, sizeof(double));
    dados_tempo =  (int*)calloc(size, sizeof(int));
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
  free(dados_torque);
  free(dados_tempo);
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
    WiFi.mode(WIFI_STA);
    bool res = wm.autoConnect("CicloErgometro"); 
    
    if(!res) { 
        Serial.println("Falha ao conectar"); 
        ESP.restart(); //reseta o esp caso nao tenha sido possivel conectar
    }
    else {  
      Serial.println("Conectado!"); //verifica se foi possivel conectar ao wifi
    } 

    Serial.println("");
    Serial.print("Rede: ");
    Serial.println(WiFi.SSID());
    Serial.print("Endereco de IP: ");
    Serial.println(WiFi.localIP());

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

void resetWifi() { //reseta as informacoes de wifi salvas
  Serial.println("Resetando o wifi");
  wm.resetSettings();
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
  //lcd.noBacklight();
  last_t = millis();

  joy = new Joystick(pot_pin, adc_register, wifi_register);
  cur = new current_sensor(acs_pin, 18, 20);
  cur->init();
  csv = new CSV();
  web = new WEBSITE();
  saved = new Memory();
    
}

// Get frequecy of pot (incluir na classe do potenciometro)
int mapPotValueToRPM(int potValue) {
  return map(potValue, 0, 4095, 0, 50);  // rpm
}

// Control to evit Overflow
bool resetEncoderIfExceedsLimit() {
  if (current_pulses > MAX_ENCODER_VALUE || current_pulses < -MAX_ENCODER_VALUE) {
    Serial.println("\n encoder pulses reset");
    encoder->setPulses(0);
    last_pulses = 0;
    return true;
  }
  return false;
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
    Serial.print("going left");
    output = max(output, -MAX_PWM);
    Serial.println(output);
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

  while (!lcd_timer.isReady()) {
    if (rpmTime.getTimePassed() > sample_t) {
      current_pulses = encoder->getPulses();
      delta_pulses = current_pulses - last_pulses;
      double revolutions = delta_pulses/pulses_per_rev;
      actual_rpm = -revolutions*(60000/400);
      //actual_rpm = delta_pulses * 1.01;

      rpmTime.reset();
      last_pulses = current_pulses;

      resetEncoderIfExceedsLimit();
      if(contador <= MAX_SAMPLES) {
        lista_values[contador] = actual_rpm;
        tempo[contador]=contador*sample_t;
        contador++;
      }
      Serial.print(actual_rpm);
      Serial.print(", ");
      Serial.println(lcd_timer.getTimePassed());

      controlMotorSpeedWithPID();
    }
    
    printTime();
  }
  saved->push_passivo(tempo, lista_values, contador + 1);
}

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


    if (verificationResistivo() < 1) {
        delay(500);
        return;
    }


    resistivo();
    print_torque_results();
    delay(500);
    Serial.println("logica executada");
}

int def_pwm_motor() {
    int pwm_motor = 50;  // Inicialize o valor do PWM
    lcd.setCursor(0, 0);
    lcd.print("              ");
    
    while (!btn->getPress()) {
        //Serial.println("Botão não apertado");
        if (joy->middle()) {
            // Serial.println("Middle");
            joystick_check = true;
        }
        if (joy->left() && joystick_check) {
            // Serial.println("Left");
            if (pwm_motor >= 10) {
                pwm_motor -= 10;
                joystick_check = false;
            }
        } else if (joy->right()  && joystick_check) {
            // Serial.println("Right");
            if (pwm_motor < 100) {
                pwm_motor += 10;
                joystick_check = false;
            }
        }
        lcd.setCursor(0, 0);
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
    torque_Time.reset();
    motorController->Set_L(pwm_motor);

    while (!lcd_timer.isReady() ) {
      acs = cur->get_current();
      torque = cur->get_torque(acs);
      if (torque_Time.getTimePassed() > sample_t) {
          if (contador <= MAX_SAMPLES) {
              //Serial.println("ok");
              //Serial.print(torque);
              //Serial.print(", ");
              //Serial.println(lcd_timer.getTimePassed());
              lista_values[contador] = torque;
              tempo[contador] = contador * sample_t;
              contador++;
              torque_Time.reset();
          }
      }
      lcd.setCursor(1, 1);
      lcd.print("TORQUE: ");
      sprintf(t, "%02f", torque);
      lcd.print(t);
      printTime();
    }

    saved->push_resistivo(tempo, lista_values, contador + 1);
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

    if (rpmTime.getTimePassed() > 400) {
      current_pulses = encoder->getPulses();
      Serial.print("; current_pulses: ");
      Serial.print(current_pulses);
      delta_pulses = current_pulses - last_pulses;
      Serial.print("; delta_pulses: ");
      Serial.print(delta_pulses);
      double revolutions = delta_pulses/pulses_per_rev;
      actual_rpm = revolutions*(60000/400);
      
      rpmTime.reset();
      last_pulses = current_pulses;
      Serial.print("; actual rpm: ");
      Serial.print(actual_rpm);
      Serial.print("; time: ");
      Serial.println(lcd_timer.getTimePassed());

      if(resetEncoderIfExceedsLimit()) continue;
      if(contador < MAX_SAMPLES) {

        lista_values[contador] = actual_rpm;
        tempo[contador]=contador*sample_t;
        contador++;
      }

    }
    lcd.setCursor(0, 1);
    lcd.print("Frequency: ");
    lcd.print(actual_rpm);
    printTime();
  }
  saved->push_normal(tempo, lista_values, contador + 1);
}

void setMode() {
  double pageSelect = 0;
  lcd.clear();
  lcd.print("Escolha o modo ");
  while (!btn->getPress()){
    if (joy->middle()) {
      joystick_check=true;
    } else if (joy->left() && joystick_check) {
      joystick_check = false;
      if(pageSelect > 0 ){
        pageSelect--;
      } else {
        pageSelect = 3;
      }
    } else if (joy->right() && joystick_check) {
      joystick_check = false;
      if(pageSelect < 3) {
          pageSelect++;
      } else {
        pageSelect = 0;
      }
    }
    if (pageSelect ==0 ) {
      STATE = NORMAL;
      lcd.setCursor(0, 1);
      lcd.print("Modo Normal   ");
    } else if (pageSelect == 1) {
      STATE = PASSIVE;
      lcd.setCursor(0, 1);
      lcd.print("Modo Passivo   ");
    } else if (pageSelect == 2) {
      STATE = FADE;
      lcd.setCursor(0, 1);
      lcd.print("Modo Resistivo   ");
    } else if (pageSelect == 3) {
      STATE = CONFIG;
      lcd.setCursor(0, 1);
      lcd.print("Configuracoes ");
    }
  }
}

// Print in LCD the MODE select
void printSelectedMode() {
  lcd.clear();
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

    case CONFIG:
      lcd.print("Selecionado:  ");
      lcd.setCursor(0, 1);
      lcd.print("Configuracoes     ");
      lcd.setCursor(0, 1);
      return;
  }
}

//Selection of the configuration options
void selectConfig() {
  double pageSelect = 0;
  lcd.clear();
  lcd.print("Configuracoes:");
    while (!btn->getPress()){
    if (joy->middle()) {
      joystick_check=true;
    } else if (joy->left() && joystick_check) {
      joystick_check = false;
      if(pageSelect > 0 ){
        pageSelect--;
      } else {
        pageSelect = 2;
      }
    } else if (joy->right() && joystick_check) {
      joystick_check = false;
      if(pageSelect < 2) {
          pageSelect++;
      } else {
        pageSelect = 0;
      }
    }

    if (pageSelect ==0 ) {
      STATE = GETIP;
      lcd.setCursor(0, 1);
      lcd.print("Obter o IP    ");
    } else if (pageSelect == 1) {
      STATE = RESETWIFI;
      lcd.setCursor(0, 1);
      lcd.print("Resetar o Wifi");
    } else if (pageSelect == 2) {
      STATE = BACK;
      lcd.setCursor(0, 1);
      lcd.print("Voltar        ");
  }
  }
}

//Implementation of the configuration tab
void configMode() {
  lcd.clear();
  lcd.setCursor(0, 0); 
  switch (STATE) {
    case GETIP:
      lcd.setCursor(0,0);
      lcd.print("IP:");
      lcd.setCursor(0,1);
      lcd.print(WiFi.localIP());
      delay(2000);
      return;
    
    case RESETWIFI:
      lcd.setCursor(0,0);
      lcd.print("Resetando o wifi...");
      delay(500);
      resetWifi();
      return;

    case BACK: //volta (reseta o esp)
      reset();
      return;
}
}

// Select Duration
int duration() {
  int t_Duration;
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
  lcd.setCursor(0, 0);
  lcd.print("Frequency: ");
  sprintf(t, "%02d", goal_rpm);
  lcd.print(t);
}


// Get frequency (Implementar como Vetor quando criar a Classe)
int goalRPM() {
  while (!btn->getPress()){
    if (!joy->right() && !joy->left())
    {
      joystick_check=true;
    }
    if (joy->left() && joystick_check)
    {
      if(goal_rpm >=80 ){
        goal_rpm-= 10;
        joystick_check = false;
      }
    }
    else if (joy->right() && joystick_check)
    {
      if(goal_rpm <= 150){
        goal_rpm+=10;
        joystick_check = false;
      }
    }   
    printFrequency();
  }
  return goal_rpm;
}


// Verification

int verificationResistivo() {
  lcd.clear();
  while (!btn->getPress()) {
    lcd.setCursor(0, 0);
    lcd.print("PWM: ");
    sprintf(t, "%02d", pwm_motor);
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

int verificationPassivo() {
  lcd.clear();
  while (!btn->getPress()) {
    if (joy->left())
    {
      verif = 0;
    }
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
      verif = verificationPassivo();
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
      // STATE = STAND_BY;
      lcd.clear();
      reset();
      return;

    case STAND_BY:
      setMode();
      printSelectedMode();
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
      delay(2);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FIM");
      delay(500);
      reset();
      return;

    case CONFIG:
      lcd.clear();
      selectConfig();
      configMode();
      delay(500);
      reset();
      return;
  }
}
