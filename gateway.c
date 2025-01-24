#include <esp_now.h>
#include <WiFi.h>
char ssid[] = "A";
char pass[] = "12345678";

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
    if (WiFi.status() == WL_CONNECTED)
    {
        // Envia o valor para o Blynk (Virtual Pin V1)

        Serial.print("LDR Value sent to Blynk: ");
        Serial.println(value_send);
    }
    else
    {
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

    WiFi.mode(WIFI_STA);
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        return;
    }

    Serial.print(F("Receiver initialized : "));
    Serial.println(WiFi.macAddress());

    esp_now_register_recv_cb(onDataRecv);

    temp = millis();
    temp_reconnect = millis();
    tem_send_sensor = millis();
}

void loop()
{

    if (millis() - tem_send_sensor > 1000)
    {
        sendSensor();
        tem_send_sensor = millis();
    }
}