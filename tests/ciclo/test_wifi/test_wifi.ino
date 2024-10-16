#include <WiFi.h>                // Biblioteca para o ESP32
#include <WiFiManager.h>         // Biblioteca WiFiManager para gerenciamento de Wi-Fi

WiFiServer server(80);           // Inicializa o servidor na porta 80

void setup() {
  Serial.begin(9600);

  // Inicializa o WiFiManager
  WiFiManager wifiManager;
  
  // Tente conectar-se ao último Wi-Fi salvo, se não, abre o portal de configuração 
  if (!wifiManager.autoConnect("AutoConnectAP")) {
    Serial.println("Falha na conexão e timeout");
    ESP.restart(); // Reinicia o ESP se a conexão falhar
  }

  // Conectado com sucesso à rede Wi-Fi
  Serial.println("Conectado à rede Wi-Fi!");

  // Inicia o servidor web
  server.begin();
}

void loop() {
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

          // Aqui começa a criação da página web com uma tabela HTML
          client.println("<html><head><title>Tabela ESP32</title></head><body>");
          client.println("<h1>Tabela de Dados</h1>");
          client.println("<table border='1'><tr><th>Item</th><th>Valor</th></tr>");

          // Exemplo de dados na tabela
          client.println("<tr><td>Temperatura</td><td>25°C</td></tr>");
          client.println("<tr><td>Umidade</td><td>60%</td></tr>");

          // Fechando a tabela e a página HTML
          client.println("</table></body></html>");

          // Quebra a conexão com o cliente após o envio da resposta
          break;
        }
      }
    }

    // Limpa a requisição e desconecta o cliente
    header = "";
    client.stop();
    Serial.println("Cliente desconectado.");
  }
}