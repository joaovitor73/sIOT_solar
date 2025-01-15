#include <WiFi.h>

// Defina as credenciais da rede Wi-Fi
const char* ssid = "A";
const char* password = "12345678";

// Defina a porta TCP (80)
const int port = 80;

// Crie o objeto servidor TCP
WiFiServer server(port);

void setup() {
  Serial.begin(115200);

  // Conectar à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede Wi-Fi...");
  }
  
  // Exibir o IP do servidor (ESP32 receptor)
  Serial.print("IP do mestre (servidor): ");
  Serial.println(WiFi.localIP());

  // Iniciar o servidor TCP
  server.begin();
  Serial.println("Servidor TCP iniciado na porta 80...");
}

void loop() {
  // Verifique se há um cliente tentando se conectar
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Cliente conectado.");

    // Esperar até que o cliente envie dados
    while (client.connected()) {
      if (client.available()) {
        // Ler os dados enviados pelo cliente
        String message = client.readStringUntil('\n');
        Serial.print("Mensagem recebida: ");
        Serial.println(message);

        // Enviar resposta para o cliente (opcional)
        client.println("Mensagem recebida com sucesso!");
      }
    }

    // Fechar a conexão quando terminar
    client.stop();
    Serial.println("Cliente desconectado.");
  }
}
