#include <WiFi.h>
#include "time.h"
#include <PubSubClient.h>

// WIFI
const char* ssid  = "MOVISTAR_4A7C";
const char* password = "CCC0774A30A6366C4B35";

// SERVIDOR NTP
const char* ntpServer = "es.pool.ntp.org";

// ZONA HORARIA
const int32_t gmtOffset_sec = 3600;
const int32_t daylightOffset_sec = 0;

// ESTRUCTURA PARA FECHA Y HORA
struct tm timeinfo;

// MQTT
const char* mqtt_username = "Uribe";
const char* mqtt_password = "1234";
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_client_id = "Ko4x8LEAIm";  // Mismo Client ID que tu app

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void setup() {
  Serial.begin(115200);

  // Conexión WiFi
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConexión WiFi establecida.");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configurar NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Configurar MQTT
  mqtt_client.setServer(mqtt_server, mqtt_port);
}

void reconnect() {
  while (!mqtt_client.connected()) {
    Serial.print("Conectando al broker MQTT...");
    if (mqtt_client.connect(mqtt_client_id, mqtt_username, mqtt_password)) {
      Serial.println("Conectado.");
    } else {
      Serial.print("Falló, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(". Intentando de nuevo en 5 segundos.");
      delay(5000);
    }
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado. Reiniciando...");
    ESP.restart();
  }

  if (!mqtt_client.connected()) {
    reconnect();
  }
  mqtt_client.loop();

  // SOLO publicamos si conseguimos obtener la hora
  if (getLocalTime(&timeinfo)) {
    String currentTime = String(timeinfo.tm_mday) + "/" + 
                         String(timeinfo.tm_mon + 1) + "/" + 
                         String(timeinfo.tm_year + 1900) + " (" + 
                         String(timeinfo.tm_hour) + ":" + 
                         String(timeinfo.tm_min) + ":" + 
                         String(timeinfo.tm_sec) + ")";
    
    Serial.println(currentTime);
    mqtt_client.publish("sensor/hora", currentTime.c_str(), true);

  } else {
    Serial.println("No se pudo obtener la hora, no se publica.");
  }

  delay(1000); // Espera de 1 segundo
}