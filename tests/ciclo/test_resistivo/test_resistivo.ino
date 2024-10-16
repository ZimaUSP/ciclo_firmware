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
 WiFiManager wifiManager;
bool wifiOn = false; 

#define MAX_SAMPLES 1000

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
int offset, pot, sum, i, contador,verif, t_Duration,pwm_motor;

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
    int pwm_motor;  // Inicialize o valor do PWM
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
    if (analogRead(pot_pin)==0)
    {
      verif = 0;
    }
    if (analogRead(pot_pin)==4095)
    {
      verif = 1;
    }
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
    contador = 0;
    // Resetar o array de torques
    for (int i = 0; i < MAX_SAMPLES; i++) {
        lista_torque[i] = 0.0;
        tempo[i]=0;
    }
    lcd_timer.reset();
    Motor->Set_L(pwm_motor);

    while (!lcd_timer.isReady()) {
        if (torque_Time.getTimePassed() > sample_t * 50) {
            if (contador < MAX_SAMPLES) {
                Serial.println("ok");
                acs = cur->get_current();
                torque = cur->get_torque(acs);
                Serial.print(torque);
                Serial.print(", ");
                Serial.println(lcd_timer.getTimePassed());
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

    // Configuração Wi-Fi com WiFiManager
    if (!wifiManager.autoConnect("AutoConnectAP")) {
        Serial.println("Falha na conexão e timeout");
        ESP.restart(); // Reinicia o ESP se a conexão falhar
    }

    // Conectado com sucesso à rede Wi-Fi
    Serial.println("Conectado à rede Wi-Fi!");

    // Inicia o servidor web
    server.begin();

    // Desliga o Wi-Fi, pois não é necessário no início
    WiFi.mode(WIFI_OFF);
    wifiOn = false;

    // Inicialização de outros componentes
    Motor = new H_bridge_controller(r_pin, l_pin, PWM_frequency_channel, PWM_resolution_channel, R_channel, L_channel);
    Motor->init();
    Motor->Set_R(0);
    lcd.init();
    btn = new Button(btn_pin, 2);
    btn->init();
    cur = new current_sensor(acs_pin, 1850, 20);
    cur->init();
}

void loop() {
    // Parte da lógica principal, sem Wi-Fi
    if (wifiOn) {
        // Desliga o Wi-Fi para não interferir
        WiFi.mode(WIFI_OFF);
        wifiOn = false;
        Serial.println("Wi-Fi desligado para executar a lógica.");
    }

    pwm_motor = def_pwm_motor();
    delay(500);
    lcd_timer.setInterval(duration() * 60000);
    delay(500);
    lcd.clear();
    int verif = verification();
    delay(500);
    if (verif >= 1) {
        delay(500);
    } else {
        delay(500);
        return;
    }

    resistivo();
    print_torque_results();
    delay(1000);

    // Lógica principal concluída, agora liga o Wi-Fi para a parte da web
    if (!wifiOn) {
        Serial.println("Ligando o Wi-Fi...");
        WiFi.mode(WIFI_STA);
        WiFi.begin();  // Reinicia a conexão Wi-Fi
        wifiOn = true;

        // Aguarde um tempo para garantir que o ESP32 se conecte à rede
        unsigned long startTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {  // Aumentado para 30 segundos
            delay(500);
            Serial.print(".");  // Para debug, imprime ponto a cada tentativa
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nConectado ao Wi-Fi, IP: " + WiFi.localIP().toString());
            server.begin();  // Reinicia o servidor
        } else {
            Serial.println("\nNão conseguiu conectar ao Wi-Fi.");
            return;  // Retorna para tentar de novo no próximo ciclo
        }
    }

    // Verifica se há clientes conectados ao servidor
    WiFiClient client = server.available();
    if (client) {
        Serial.println("Novo cliente conectado!");
        String header;

        // Espera até o cliente enviar alguma informação
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                header += c;

                // Verifica se a requisição foi encerrada
                if (c == '\n') {
                    // Envia resposta HTTP (cabeçalho e conteúdo)
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println();

                    // Começa a criar a página web com a tabela HTML
                    client.println("<html><head><title>Tabela ESP32</title></head><body>");
                    client.println("<h1>Tabela de Dados de Torque e Tempo</h1>");
                    client.println("<table border='1'><tr><th>Tempo (ms)</th><th>Torque</th></tr>");

                    // Itera sobre os valores dos arrays e os exibe na tabela
                    for (int i = 0; i < MAX_SAMPLES; i++) {
                        client.print("<tr><td>");
                        client.print(tempo[i]);  // Exibe o tempo
                        client.print("</td><td>");
                        client.print(lista_torque[i], 2);  // Exibe o torque com 2 casas decimais
                        client.println("</td></tr>");
                    }

                    // Fecha a tabela e a página HTML
                    client.println("</table></body></html>");

                    // Fecha a conexão com o cliente após o envio da resposta
                    break;
                }
            }
        }

        // Fecha a conexão
        client.stop();
        Serial.println("Cliente desconectado.");
    }

    // Após servir a página, desliga o Wi-Fi novamente para não interferir
    WiFi.mode(WIFI_OFF);
    wifiOn = false;
    Serial.println("Wi-Fi desligado após servir a página web.");
}