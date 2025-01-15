/*********************
  Example: Send potentiometer data to Blynk
 *********************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL2FsxMw0-3"
#define BLYNK_TEMPLATE_NAME         "Teste"
#define BLYNK_AUTH_TOKEN            "TUkI8hbMhTZW1-un4wv4peUu7Rjr5ZZY"
#include <SoftwareSerial.h>

// Configura SoftwareSerial nos pinos 10 e 11
SoftwareSerial mySerial(10, 11); // RX = pino 10, TX = pino 11

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

char auth[] = "TUkI8hbMhTZW1-un4wv4peUu7Rjr5ZZY";
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// Endereço MAC (verifique a etiqueta do Ethernet Shield ou use um endereço aleatório)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// IP fixo (opcional, pode usar DHCP)
IPAddress ip(10,77,12,103);  // Ou deixe o DHCP configurar automaticamente

BlynkTimer timer;

// Pino analógico onde o potenciômetro está conectado
const int potPin = A0;  // Conecte o potenciômetro ao pino A0 ou outro pino analógico

// Função para enviar os dados do potenciômetro para o Blynk
void sendPotentiometerData()
{
  // Lê o valor do potenciômetro (0 a 1023)
  int sensorValue = map(analogRead(potPin),0,1023,0,255);

  Serial.println(sensorValue);
  
  // Envia o valor para o pino virtual V1 no Blynk (pode ser ajustado conforme necessidade)
  Blynk.virtualWrite(V1,sensorValue);  // Mapeia o valor de 0-1023 para 0-255, se necessário

  Blynk.virtualWrite(V2,sensorValue);  // Mapeia o valor de 0-1023 para 0-255, se necessário

}

// Função chamada toda vez que o dispositivo se conecta ao Blynk.Cloud
BLYNK_CONNECTED()
{
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// Função de temporizador para enviar dados a cada 1 segundo
void myTimerEvent()
{
  sendPotentiometerData();  // Chama a função para enviar os dados do potenciômetro
}

void setup()
{
  // Inicializa a porta serial para debug
  Serial.begin(115200);

  mySerial.begin(9600);
  // Inicializa a Ethernet (DHCP ou IP fixo)
   // Para DHCP: Ethernet.begin(mac);
  
  // Inicia o Blynk
  Blynk.begin(auth);
  pinMode(potPin, INPUT);
  // Configura o temporizador para enviar dados do potenciômetro a cada 1 segundo
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
  if (mySerial.available()) {
    String receivedData = mySerial.readStringUntil('\n'); // Lê os dados até '\n'
    if (receivedData == "Desconectado") {
       Ethernet.begin(mac, ip);  // Envia os dados do potenciômetro
    }
  }
}
