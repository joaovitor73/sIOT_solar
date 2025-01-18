#define BLYNK_TEMPLATE_ID "TMPL2FsxMw0-3"
#define BLYNK_TEMPLATE_NAME "Teste"
#define BLYNK_AUTH_TOKEN "TUkI8hbMhTZW1-un4wv4peUu7Rjr5ZZY"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "A";
char pass[] = "12345678";

const char *ssid_esp = "ESP32_AP"; // Nome da rede criada pelo ESP32 (AP)
const char *pass_esp = "12345678"; // Senha da rede criada pelo ESP32 (AP)

bool blynk_connect = false;
BlynkTimer timer;
unsigned long temp;
unsigned long temp_reconnect = 0;

#define LDR_PIN 34

HardwareSerial SerialPort(2);
int cont = 0;
void sendSensor()
{
  cont += 1;
  // Leitura do valor analógico do LDR
  int ldrValue = analogRead(LDR_PIN);
  ldrValue = cont;
  // Verifica se está conectado ao Wi-Fi e ao Blynk
  if (WiFi.status() == WL_CONNECTED && Blynk.connected())
  {
    // Envia o valor para o Blynk (Virtual Pin V1)
    Blynk.virtualWrite(V1, ldrValue);

    // Mensagem para depuração
    Serial.print("LDR Value sent to Blynk: ");
    Serial.println(ldrValue);
  }
  else
  {
    // Envia os dados via UART para o Arduino
    Serial.print("Sending to Arduino via UART: ");
    Serial.println(ldrValue);
    SerialPort.println(ldrValue); // Envia o valor pela UART
  }
}

void setup()
{
  Serial.begin(115200); // Inicializa o monitor serial
  Serial.println("Starting ESP32...");

  SerialPort.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("UART2 inicializada");
  // Conecta ao Blynk e Wi-Fi
  Serial.println("Connecting to Wi-Fi...");

  WiFi.mode(WIFI_AP_STA);
  WiFi.persistent(false);      // Impede que o ESP32 salve credenciais na memória flash
  WiFi.setAutoReconnect(true); // Habilita a reconexão automática
  WiFi.begin(ssid, pass);      // Inicia a conexão Wi-Fi

  bool apStarted = WiFi.softAP(ssid_esp, pass_esp);
  if (apStarted)
  {
    Serial.println("Access Point criado com sucesso!");
    Serial.print("Nome da rede (AP): ");
    Serial.println(ssid_esp);
    Serial.print("Endereço IP (AP): ");
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println("Falha ao criar o Access Point.");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    blynk_connect = true;
    Blynk.begin(auth, ssid, pass);
  }

  // Configura o timer para enviar os dados a cada 1 segundo (1000ms)
  timer.setInterval(1000L, sendSensor);

  Serial.println("ESP32 setup completed.");

  Serial.println("\nConectado à rede Wi-Fi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  temp = millis();
  temp_reconnect = millis();
}

void loop()
{
  // Executa as funções do Blynk se estiver conectado
  if (blynk_connect && WiFi.status() == WL_CONNECTED)
  {
    Blynk.run();
  }
  else
  {
    blynk_connect = false;
  }

  // Executa o timer independentemente da conexão
  timer.run();

  if (!blynk_connect && WiFi.status() == WL_CONNECTED)
  {
    blynk_connect = true;
    Blynk.begin(auth, ssid, pass);
  }

  temp = millis();
  if ((temp - temp_reconnect >= 30000))
  {
    if ((WiFi.status() != WL_CONNECTED))
    {
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.reconnect();
    }
    temp_reconnect = temp;
  }
}