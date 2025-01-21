#define BLYNK_TEMPLATE_ID "TMPL2FsxMw0-3"
#define BLYNK_TEMPLATE_NAME "Teste"
#define BLYNK_AUTH_TOKEN "TUkI8hbMhTZW1-un4wv4peUu7Rjr5ZZY"

#define BLYNK_PRINT Serial
#include <esp_now.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "A";
char pass[] = "12345678";

bool blynk_connect = false;
BlynkTimer timer;
unsigned long temp;
unsigned long temp_reconnect = 0;
unsigned long tem_send_sensor = 0;

HardwareSerial SerialPort(2);
int cont = 0;
int ldrValue = 0;
volatile int value_send = 0;
void sendSensor()
{
  // cont += 1;
  // Leitura do valor analógico do LDR
  // ldrValue = cont;
  // Verifica se está conectado ao Wi-Fi e ao Blynk
  if (WiFi.status() == WL_CONNECTED && Blynk.connected())
  {
    // Envia o valor para o Blynk (Virtual Pin V1)
    Blynk.virtualWrite(V1, value_send);

    Serial.print("LDR Value sent to Blynk: ");
    Serial.println(value_send);
  }
  else
  {
    Serial.println(Blynk.connected());
    // Envia os dados via UART para o Arduino
    Serial.print("Sending to Arduino via UART: ");
    Serial.println(value_send);
    SerialPort.println(value_send);
  }
}

void onDataRecv(const esp_now_recv_info *info, const uint8_t *data, int len)
{
  if (len == sizeof(int))
  {
    memcpy(&ldrValue, data, sizeof(ldrValue));
    value_send = ldrValue;
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
  WiFi.persistent(false);
  WiFi.setAutoReconnect(true);
  // WiFi.begin(ssid, pass);

  // if (WiFi.status() == WL_CONNECTED)
  // {
  //   blynk_connect = true;
  //   Blynk.begin(auth, ssid, pass);
  // }

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  Serial.print(F("Receiver initialized : "));
  Serial.println(WiFi.macAddress());

  esp_now_register_recv_cb(onDataRecv);
  Serial.println("ESP32 setup completed.");

  Serial.println("\nConectado à rede Wi-Fi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  temp = millis();
  temp_reconnect = millis();
  tem_send_sensor = millis();
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

  if (millis() - tem_send_sensor > 1000)
  {
    sendSensor();
    tem_send_sensor = millis();
  }
}