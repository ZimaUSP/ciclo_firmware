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
#include <WiFiManager.h>         // Biblioteca WiFiManager para gerenciamento de Wi-Fi

WiFiServer server(80);  

bool wifiOn = false; 

#define MAX_SAMPLES 1000

const char* ssid = "iPhone";     // Substitua pelo nome da sua rede Wi-Fi
const char* password = "kess@123"; // Substitua pela senha da rede

H_bridge_controller *Motor;
current_sensor *cur;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SimpleTimer lcd_timer;
SimpleTimer torque_Time;
double lista_torque[MAX_SAMPLES];
int tempo[MAX_SAMPLES];
Button* btn;

double t0, torque, torque_max, torque_min, torque_med;
float acs;
bool joystick_check;
char t[10];  // Aumentado o tamanho do buffer de sprintf
int offset, pot, sum, i, contador,pwm_motor;
double  t_Duration = 0.5;
int verif = 1;



void inicializaComponentes() {
    Motor = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
    Motor->init();
    Motor->Set_R(0);

    lcd.init();
    btn = new Button(btn_pin, 2);
    btn->init();
    cur = new current_sensor(acs_pin, 1850, 20);
    cur->init();
}

void gerenciarWiFi(bool ligar) {
    if (ligar && !wifiOn) {
        conectarWiFi();
    } else if (!ligar && wifiOn) {
        WiFi.mode(WIFI_OFF);
        wifiOn = false;
        Serial.println("Wi-Fi desligado.");
    }
}

void executarLogica() {
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
    delay(1000);
}

void conectarWiFi() {
    Serial.println("Conectando ao Wi-Fi...");
    WiFi.begin(ssid, password);

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConectado! IP: " + WiFi.localIP().toString());
        server.begin(); // Inicia o servidor
        wifiOn = true;
    } else {
        Serial.println("\nFalha ao conectar. Reiniciando...");
        ESP.restart(); // Reinicia o ESP caso falhe
    }
}


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

int duration() {
  lcd.setCursor(0, 0);
  lcd.print("              ");
  while (!btn->getPress()){
         if (analogRead(pot_pin) >= 1000 && analogRead(pot_pin) <= 3000)
            {
                joystick_check=true;
            }
            if (analogRead(pot_pin) <1000 && joystick_check)
            {
                if(t_Duration !=0 ){
                    t_Duration--;
                    joystick_check = false;
                }
            }
            else if (analogRead(pot_pin) > 3000 && joystick_check)
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
        if (analogRead(pot_pin) >=1000 && analogRead(pot_pin) <= 3000) {
            joystick_check = true;
        }
        if (analogRead(pot_pin) <1000 && joystick_check) {
            if (pwm_motor >= 10) {
                pwm_motor -= 10;
                joystick_check = false;
            }
        } else if (analogRead(pot_pin) > 3000  && joystick_check) {
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
        lista_torque[i] = 0.0;
        tempo[i]=0;
    }
    lcd_timer.reset();
    Motor->Set_L(pwm_motor);

    while (!lcd_timer.isReady() ) {
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
        delay(10);  // Evitar loop sem pausa
    }
    lcd.clear();
    delay(10);
    lcd.setCursor(0, 0);
    lcd.print("Fim");
    delay(1000);
    lcd.clear();
}

void setup() {
    Serial.begin(9600);
    // Conectar ao Wi-Fi na inicialização
    conectarWiFi();

    // Inicializa outros componentes
    inicializaComponentes();

    // Desliga o Wi-Fi após a inicialização, se necessário
    //gerenciarWiFi(true);
}

void loop() {
    // Executa a lógica principal com o Wi-Fi desligado
    executarLogica();

    // Liga o Wi-Fi para servir a página da web
    //gerenciarWiFi(true);

    // Verifica e atende a clientes conectados ao servidor
    atenderClientes();

    // Desliga o Wi-Fi após atender a página da web
    //gerenciarWiFi(false);
}