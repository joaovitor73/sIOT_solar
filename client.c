#include <WiFi.h>

// Defina as credenciais da rede Wi-Fi
const char* ssid = "A";
const char* password = "12345678";

// Endereço IP do ESP32 receptor (mestre) - use o IP mostrado no monitor serial do servidor
const char* serverIP = "192.168.43.178";  // Substitua pelo IP mostrado no servidor
const int port = 80;  // A mesma porta do servidor (porta 80)

const byte ldrpin = 5;  // Pino de leitura do LDR
// Crie o objeto cliente TCP
WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Conectar à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede Wi-Fi...");
  }
  Serial.println("Conectado à rede Wi-Fi!");
}

void loop() {
  // Conectar ao servidor (ESP32 receptor) na porta 80
  if (client.connect(serverIP, port)) {
    Serial.println("Conectado ao servidor!");

    // Ler o valor do LDR
    int ldrValue = analogRead(ldrpin);

    // Mapear o valor do LDR para a faixa de 0 a 255
    int mappedValue = map(ldrValue, 0, 4095, 0, 255);
    
    // Exibir no Monitor Serial o valor mapeado
    Serial.print("Valor LDR mapeado: ");
    Serial.println(mappedValue);

    // Enviar o valor mapeado para o servidor
    client.println(mappedValue);  // Enviar o valor mapeado para o servidor
    Serial.println("Mensagem enviada!");

    // Esperar pela resposta do servidor
    while (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.print("Resposta do servidor: ");
      Serial.println(response);
    }

    // Fechar a conexão
    client.stop();
    Serial.println("Conexão fechada.");
  } else {
    Serial.println("Falha na conexão com o servidor.");
  }

  delay(10);  
}
