#include <SPI.h>
#include <Ethernet.h>

// Defina o endereço MAC manualmente ou deixe o Arduino gerar automaticamente
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // Exemplo de MAC

EthernetClient client;

void setup() {
  Serial.begin(9600);

  // Inicializa a Ethernet usando o endereço MAC fornecido
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha na configuração do DHCP. Usando IP fixo.");
    Ethernet.begin(mac, IPAddress(192, 168, 1, 100));  // IP fixo como fallback
  }

  // Aguarda 1 segundo para garantir que a Ethernet seja inicializada
  delay(1000);

  // Mostra o IP e o endereço MAC no Monitor Serial
  Serial.print("Endereço IP: ");
  Serial.println(Ethernet.localIP());
  
  // Aqui não há uma função direta para exibir o MAC, então imprimimos o que configuramos
  Serial.print("Endereço MAC: ");
  for (byte i = 0; i < 6; i++) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i < 5) {
      Serial.print(":");
    }
  }
  Serial.println();
}

void loop() {
  // Nada para fazer no loop
}
