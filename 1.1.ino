#include <WiFi.h>
#include "time.h"
#include <stdint.h>

// WIFI
const char* ssid  = "MOVISTAR_4A7C";
const char* password = "CCC0774A30A6366C4B35";

// SERVIDOR PARA FECHA Y HORA
const char* ntpServer = "es.pool.ntp.org";

// ZONA HORARIA
const int32_t gmtOffset_sec = 3600;

// HORARIO DE VERANO
const int32_t daylightOffset_sec = 0;

// ESTRUCTURA PARA TRABAJAR CON NTP
struct tm timeinfo;

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

//FECHA Y HORA
configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void loop() {
  getLocalTime(&timeinfo);
  Serial.println(&timeinfo, "%d %m %Y %H:%M:%S");
  delay (1000);

}
