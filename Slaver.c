#include <WiFi.h>


const char* ssid = "A"; 
const char* password = "12345678"; 

HardwareSerial SerialPort(2); 

void setup() {
  Serial.begin(115200); 
  Serial.println("Iniciando ESP32...");

  SerialPort.begin(9600, SERIAL_8N1, 16, 17); 
  Serial.println("UART2 inicializada");

  Serial.println("Conectando à rede Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    SerialPort.println("Tentando conectar ao Wi-Fi...");
  }

  Serial.println("\nConectado à rede Wi-Fi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  SerialPort.println("Conectado à rede Wi-Fi!"); 
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    SerialPort.println("Conectado");
  } else {
    SerialPort.println("Desconectado");
  }
  delay(1000); 
}
