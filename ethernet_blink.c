/*********************
  Exemplo: Receber dados do LDR via UART e enviar para o Blynk
 *********************/

/* Preencha as informações do dispositivo Blynk */
#define BLYNK_TEMPLATE_ID "TMPL2FsxMw0-3"
#define BLYNK_TEMPLATE_NAME "Teste"
#define BLYNK_AUTH_TOKEN "TUkI8hbMhTZW1-un4wv4peUu7Rjr5ZZY"

#include <SoftwareSerial.h>
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// Configura SoftwareSerial nos pinos 10 e 11
String receivedMessage = ""; // Variable to store the complete message

/* Define para habilitar prints para depuração */
#define BLYNK_PRINT Serial

bool send_blynk = false;
volatile bool *send_blynk_pointer = &send_blynk;
unsigned long time;

// Credenciais Blynk
char auth[] = BLYNK_AUTH_TOKEN;

// Endereço MAC (use um endereço fixo ou aleatório para o Ethernet Shield)
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Endereço IP fixo (opcional, pode usar DHCP)
IPAddress ip(10, 77, 12, 103); // Ajuste conforme sua rede

// Inicializa o temporizador do Blynk
BlynkTimer timer;

// Variável para armazenar o valor do LDR recebido via UART
int ldrValue = 0;

// Função para enviar os dados do LDR para o Blynk
void sendLdrDataToBlynk()
{
  // Envia o valor do LDR para o pino virtual V1 no Blynk
  if (*send_blynk_pointer)
  {
    Blynk.virtualWrite(V1, ldrValue);

    // Debug no monitor serial
    Serial.print("Valor do LDR enviado para o Blynk: ");
    Serial.println(ldrValue);
  }
}

// Função chamada toda vez que o dispositivo se conecta ao Blynk.Cloud
BLYNK_CONNECTED()
{
  // Configura propriedades do botão no Blynk
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// Função de temporizador para enviar dados a cada 1 segundo
void myTimerEvent()
{
  sendLdrDataToBlynk(); // Envia os dados do LDR
}

void setup()
{
  // Inicializa a porta serial para depuração
  Serial.begin(115200);

  // Inicializa o Blynk
  Blynk.begin(auth);

  // Executa o Blynk
  Serial.println("Blynk iniciado");
  // Configura o temporizador para enviar dados do LDR a cada 1 segundo
  timer.setInterval(1000L, myTimerEvent);

  Serial.println("Arduino iniciado e aguardando dados do ESP32 via UART.");
  time = millis();
}

void loop()
{
  // Executa o Blynk
  Blynk.run();

  // Executa o temporizador
  timer.run();
  // Verifica se há dados recebidos via UART
  if (Serial.available())
  {
    String receivedData = Serial.readStringUntil('\n'); // Lê os dados até '\n'
    ldrValue = receivedData.toInt();
    send_blynk = true;
    Serial.println(ldrValue);
  }
  else if (millis() - time > 1000)
  {
    send_blynk = false;
    time = millis();
  }
}