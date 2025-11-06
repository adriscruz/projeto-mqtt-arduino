#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";
const int   mqtt_port   = 1883;
const char* mqtt_topic  = "adriana/temperatura";
const int sensorPin     = 34;
const int ledVerde      = 25;
const int ledVermelho   = 26;
const int buzzer        = 27;

WiFiClient espClient;
PubSubClient client(espClient);

void conectaWiFi(){
  Serial.print("Conectando ao WiFi ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("IP obtido: ");
  Serial.println(WiFi.localIP());
}

void conectaMQTT(){
  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()){
    Serial.print("Conectando ao broker MQTT… ");
    if (client.connect("ESP32ClientTempratura")){
      Serial.println("conectado.");
      client.publish(mqtt_topic, "Cliente conectado!");
    } else {
      Serial.print("erro, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);

  conectaWiFi();
  conectaMQTT();
}

void loop() {
  if (!client.connected()){
    conectaMQTT();
  }
  client.loop();

  int leitura = analogRead(sensorPin);
  float tensao = leitura * (3.3 / 4095.0);
  float resistencia = (3.3 * 10000.0 / tensao) - 10000.0;
  float temperatura = 1 / (log(resistencia / 10000.0) / 3950.0 + 1.0 / 298.15) - 273.15;


  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  char payload[50];
  snprintf(payload, 50, "Temperatura = %.2f °C", temperatura);
  client.publish(mqtt_topic, payload);

  if (temperatura >= 37.5) {
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);
    tone(buzzer, 1000);
  } else {
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH);
    noTone(buzzer);
  }

  delay(5000);
}
