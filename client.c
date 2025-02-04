#include <esp_now.h>
#include <WiFi.h>
uint8_t gatewayAddress[] = {0x3C, 0x71, 0xBF, 0x4F, 0xC3, 0x5C};
const byte ldrpin = 34; // Pino de leitura do LDR
void sendData(int valueToSend)
{
  esp_err_t result = esp_now_send(gatewayAddress, (uint8_t *)&valueToSend, sizeof(valueToSend));
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK)
  {

    return;
  }

  // Adicionar o ESP Principal como peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, gatewayAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    return;
  }
}

void loop()
{
  // Ler o valor do LDR
  int ldrValue = analogRead(ldrpin);

  // Mapear o valor do LDR para a faixa de 0 a 255
  int mappedValue = map(ldrValue, 0, 4095, 0, 255);
  sendData(mappedValue);
  // Exibir no Monitor Serial o valor mapeado
  Serial.println(mappedValue);

  delay(1000);
}
