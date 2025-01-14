#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
IPAddress ip(10,77,12,103); 


EthernetServer server(80);

void setup() {
  
  Ethernet.begin(mac, ip);
  server.begin();

  
  Serial.begin(9600);
  delay(1000);  
  Serial.println("Servidor Web Iniciado!");
  Serial.print("Endereço IP do Arduino: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // Aguarda a conexão do browser
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Novo dispositivo conectado");
    Serial.print("IP do Cliente: ");
    Serial.println(client.remoteIP()); // Mostra o IP do cliente que se conectou

    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);  // Imprime os caracteres recebidos no Monitor Serial

        // Quando a solicitação HTTP terminar, envia a resposta
        if (c == '\n' && currentLineIsBlank) {
          // Envia o cabeçalho HTTP para o navegador
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<meta charset=\"utf-8\">");
          client.println("<title>Shield Ethernet</title>");
          client.println("</head>");
          client.println("<body>");
          // Configura o texto e o imprime no navegador
          client.print("<font color=#FF0000>");
          client.print("Teste de WebServer com Ethernet Shield e Arduino Usinainfo.");
          client.print("</font>");
          client.println("<br />");
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // Começando uma nova linha
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // Novo caractere na linha atual
          currentLineIsBlank = false;
        }
      }
    }

    // Tempo para o navegador receber as informações
    delay(1);

    // Fecha a conexão com o cliente
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
