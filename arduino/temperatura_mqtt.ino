#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LM35 A0
#define LED_VERMELHO 3
#define LED_VERDE 4
#define BUZZER 5

// ======= Configurações de Wi-Fi =======
const char* ssid = "SEU_WIFI";
const char* password = "SENHA_WIFI";

// ======= Configurações de MQTT =======
const char* mqtt_server = "test.mosquitto.org";
WiFiClient espClient;
PubSubClient client(espClient);

// ======= Função para conectar ao Wi-Fi =======
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.println(WiFi.localIP());
}

// ======= Setup =======
void setup() {
  Serial.begin(115200);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

// ======= Função principal =======
void loop() {
  if (!client.connected()) {
    Serial.println("Reconectando MQTT...");
    while (!client.connected()) {
      if (client.connect("ArduinoClient")) {
        Serial.println("Conectado ao MQTT!");
      } else {
        delay(2000);
      }
    }
  }

  client.loop();

  float valor = analogRead(LM35);
  float temperatura = (valor * 5.0 / 1023.0) * 100.0;

  Serial.print("Temperatura: ");
  Serial.println(temperatura);

  // Publica no tópico
  char msg[50];
  sprintf(msg, "%.2f", temperatura);
  client.publish("adriana/temperatura", msg);

  // Controle dos atuadores
  if (temperatura >= 37.5) {
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED_VERDE, LOW);
  } else {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(BUZZER, LOW);
  }

  delay(2000); // leitura a cada 2 segundos
}
