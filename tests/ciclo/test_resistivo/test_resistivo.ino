#include <dummy.h>
#include <string>
#include "H_bridge_controller.hpp"
#include "config.hpp"
#include "current_sensor.hpp"
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>
#include <iostream>
#include <algorithm>  // Para std::max_element e std::min_element
#include <numeric>    // Para std::accumulate
#include "Button.hpp"
#include <WiFi.h>                // Biblioteca para o ESP32
//#include <WiFiManager.h>         // Biblioteca WiFiManager para gerenciamento de Wi-Fi

#include <WiFiClient.h>
#include <WebServer.h>

#include <ArduinoJson.h>
#include "config.hpp"

#include "soc/sens_reg.h" // needed for manipulating ADC2 control register
#include "Joystick.hpp"
#include "CSV.hpp"
#include "Memory.hpp"
#include "Website.hpp"

Memory* database;
Joystick *joystick;
CSV *csv;
Memory* saved;
WEBSITE* web;

uint32_t adc_register;
uint32_t wifi_register;

WebServer server(80);  

//bool wifiOn = false; 

#define MAX_SAMPLES 5
#define N_SESSIONS 8

const char* ssid = "Zima";     // Substitua pelo nome da sua rede Wi-Fi
const char* password = "enzimasUSP"; // Substitua pela senha da rede

H_bridge_controller *Motor;
current_sensor *cur;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SimpleTimer lcd_timer;
SimpleTimer torque_Time;
Button* btn;

double t0, torque, torque_max, torque_min, torque_med;
float acs;
bool joystick_check = false;
char t[10];  // Aumentado o tamanho do buffer de sprintf
int offset, pot, sum, i, contador,pwm_motor;
double  t_Duration = 0.5;
int verif = 1;
int n_sessions;
double lista_torque [MAX_SAMPLES];
int tempo [MAX_SAMPLES];

bool done = false;

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
  //n_sessions = saved->get_saved_sessions_resistivo();
  JsonDocument doc; // cria o documento em formato json 

  JsonArray sessions = doc["sessions"].to<JsonArray>(); // cria o objeto sessions
  sessions.add(n_sessions); // adiciona valor de sessions para o objeto json sessions

  String output; // cria uma string chamada output

  doc.shrinkToFit();  // optional

  serializeJson(doc, output); //serializa o objeto (formata ele para string)

  server.send(200, "text/json", output);
}

void endpoints(){
  server.on("/data/resistivo/sessions", getData); //pega dados resistivo
  server.on("/data/passivo/sessions", getData); //pega dados passivo
  server.on("/data/normal/sessions", getData); //pega dados normal

  server.on("/number/resistivo/sessions", numberSessions); //pega o número de sessões 
  server.on("/number/passivo/sessions", numberSessions); //pega o número de sessões 
  server.on("/number/normal/sessions", numberSessions); //pega o número de sessões 
  //server.on("/oi", oi); //pega o número de sessões 
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


void inicializaComponentes() {
    Motor = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
    Motor->init();
    Motor->Set_R(0);

    lcd.init();
    btn = new Button(btn_pin, 2);
    btn->init();
    cur = new current_sensor(acs_pin, 18, 20);
    cur->init();
	  joystick = new Joystick(pot_pin, adc_register, wifi_register);
    csv = new CSV();
    web = new WEBSITE();
    
    database = new Memory(N_SESSIONS);
}

/*
void gerenciarWiFi(bool ligar) {
    if (ligar && !wifiOn) {
        conectarWiFi();
    } else if (!ligar && wifiOn) {
        WiFi.mode(WIFI_OFF);
        wifiOn = false;
        Serial.println("Wi-Fi desligado.");
    }
}
*/

void executarLogica() {

    //if (done)
    //  return;
    Serial.println("executar lógica");
    website_data();
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

    endpoints();
    //website_data();
    //server.onNotFound(handleNotFound);

    /*
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConectado! IP: " + WiFi.localIP().toString());
        server.begin(); // Inicia o servidor
        //wifiOn = true;
    } else {
        Serial.println("\nFalha ao conectar. Reiniciando...");
        ESP.restart(); // Reinicia o ESP caso falhe
    }
    */
}

/*
// Função para atender clientes do servidor
void atenderClientes() {
    Serial.println("Aguardando cliente...");
    WiFiClient client = server.available(); // Aguarda conexão

    if (client) {
        Serial.println("Novo cliente conectado!");
        String header;

        // Lê a requisição do cliente e responde com uma página HTML
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                header += c;

                if (c == '\n') {
                    enviarPagina(client); // Envia resposta
                    break;
                }
            }
        }

        client.stop(); // Fecha a conexão
        Serial.println("Cliente desconectado.");
    }
}
*/
/*
// Função para enviar a página HTML
void enviarPagina(WiFiClient &client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    client.println("<html><head><title>Tabela ESP32</title></head><body>");
    client.println("<h1>Tabela de Dados de Torque e Tempo</h1>");
    client.println("<table border='1'><tr><th>Tempo (ms)</th><th>Torque</th></tr>");

    for (int i = 0; i < MAX_SAMPLES; i++) {
        client.printf("<tr><td>%d</td><td>%.2f</td></tr>", tempo[i], lista_torque[i]);
    }

    client.println("</table></body></html>");
}
*/

int duration() {
  lcd.setCursor(0, 0);
  lcd.print("              ");
  while (!btn->getPress()){
    server.handleClient();
    //Serial.println(joystick->get_power());
    if (joystick->middle())
      {
          joystick_check=true;
      }
      if (joystick->left() && joystick_check)
      {
          Serial.println("left");
          if(t_Duration !=0 ){
              t_Duration--;
              joystick_check = false;
          }
      }
      else if (joystick->right() && joystick_check)
      {
          Serial.print("right - duration:");
          Serial.println(t_Duration);
          if(t_Duration != 10){
              t_Duration++;
              joystick_check = false;
          }
      }   
    lcd.setCursor(0, 0);  // MAX(15,1) linha, coluna
    lcd.print("Duration i: ");
    sprintf(t, "%f", t_Duration);
    lcd.print(t);
    lcd.print(":00  ");
    delay(2);
  }
  return t_Duration;
}

void printTime() {
  lcd.setCursor(0, 0);
  lcd.print("Duration: ");
  sprintf(t, "%02d", lcd_timer.getMinutes());
  lcd.print(t);
  lcd.print(":");
  sprintf(t, "%02d", lcd_timer.getSeconds());
  lcd.print(t);
}

int def_pwm_motor() {
    int pwm_motor = 50;  // Inicialize o valor do PWM
    lcd.setCursor(0, 0);
    lcd.print("              ");
    
    while (!btn->getPress()) { 
        Serial.println(joystick->get_power());
        //Serial.println("Botão não apertado");
        server.handleClient();
        if (joystick->middle()) {
            Serial.println("Middle");
            joystick_check = true;
        }
        if (joystick->left() && joystick_check) {
            Serial.println("Left");
            if (pwm_motor >= 10) {
                pwm_motor -= 10;
                joystick_check = false;
            }
        } else if (joystick->right()  && joystick_check) {
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


int verification() {
  lcd.clear();
  while (!btn->getPress()) {
    server.handleClient();
    // if (analogRead(pot_pin)==0)
    // {
    //   verif = 0;
    // }
    // if (analogRead(pot_pin)==4095)
    // {
    //   verif = 1;
    // }
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


void resistivo() {
  delay(200);
    contador = 0;
    // Resetar o array de torques
    for (int i = 0; i < MAX_SAMPLES; i++) {
        lista_torque[i] = 3.0;
        tempo[i]=3;
    }
    lcd_timer.reset();
    Motor->Set_L(pwm_motor);

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
                lista_torque[contador] = torque;
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

    database->push_resistivo(tempo, lista_torque, MAX_SAMPLES);
    delay(500);
    if (contador > 0) {  // Evitar divisão por zero
        torque_max = lista_torque[0];
        torque_min = lista_torque[0];
        double soma = 0.0;

        for (int i = 0; i < contador; i++) {
            if (lista_torque[i] > torque_max) {
                torque_max = lista_torque[i];
            }
            if (lista_torque[i] < torque_min) {
                torque_min = lista_torque[i];
            }
            soma += lista_torque[i];
        }
        // Calcular a média
        torque_med = soma / contador;
    } else {
        torque_max = 0;
        torque_min = 0;
        torque_med = 0;
    }

    delay(100);
    Motor->Set_L(0);
}

void website_data(){
    Serial.print("IP local: ");
    Serial.println(WiFi.localIP());
    server.on("/data/csv/resistivo/sessions", []() {
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
    saved->get_resistivo(id, tempo, lista_torque);
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

    Serial.begin(9600);
    // Conectar ao Wi-Fi na inicialização
    conectarWiFi();
    //endpoints();
    server.begin();

    // Inicializa outros componentes
    inicializaComponentes();

    // Desliga o Wi-Fi após a inicialização, se necessário
    //gerenciarWiFi(true);
}

void loop() {
    executarLogica();
    server.handleClient();
    delay(2);
    // Executa a lógica principal com o Wi-Fi desligado


    // Liga o Wi-Fi para servir a página da web
    //gerenciarWiFi(true);

    // Verifica e atende a clientes conectados ao servidor
    //atenderClientes();

    // Desliga o Wi-Fi após atender a página da web
    //gerenciarWiFi(false);
}
