#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

int ledPin = 4;
int aire = 5;
String ssid = "NombreRed";
String password = "Contrasena";
int estado = 0;
int estadoAire = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n");

  pinMode(ledPin, OUTPUT);
  pinMode(aire, OUTPUT);
  // digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("");
  Serial.print("Conectado a la red WiFi: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("macAdress: ");
  Serial.println(WiFi.macAddress());
  Serial.println("*");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { 
    WiFiClientSecure wifiClient; 
    wifiClient.setInsecure(); // Desactiva la verificación del certificado, solo para pruebas

    HTTPClient http; 
    http.begin(wifiClient, "https://dad3be85-1cb5-490c-9006-f6c528607b73-00-2tw3gj9c7wswc.picard.replit.dev/api/state"); 
    int httpCode = http.GET(); 

    if (httpCode > 0) { 
      String payload = http.getString(); 
      Serial.println("Respuesta del servidor:");
      Serial.println(payload); 

      DynamicJsonDocument doc(512); 
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        if (doc.containsKey("Light")) { 
          const char* lightState = doc["Light"]; 

          if (strcmp(lightState, "ON") == 0) {
            estado = 0;
          } else if (strcmp(lightState, "OFF") == 0) {
            estado = 1;
          } else {
            Serial.println("Valor desconocido para 'Light'");
          }

          digitalWrite(ledPin, estado);
          Serial.print("Estado del LED: ");
          Serial.println(estado == 1 ? "OFF" : "ON");
        } else {
          Serial.println("El JSON no contiene el campo 'Light'");
        }
        //funcionamiento aire xd
        if (doc.containsKey("air")) {
          const char* aireState = doc["air"];

          if (strcmp(aireState, "ON") == 0) {
            estadoAire = 0;
          } else if (strcmp(aireState, "OFF") == 0) {
            estadoAire = 1;
          } else {
            Serial.println("Valor desconocido para 'air'");
          }

          digitalWrite(aire, estadoAire);
          Serial.print("Estado del Aire: ");
          Serial.println(estadoAire == 1 ? "OFF" : "ON");
        } else {
          Serial.println("El JSON no contiene el campo 'air'");
        }
      } else {
        Serial.print("Error al parsear JSON: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.print("Error en la petición HTTP: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }
    http.end();
  }
delay(1000);
}