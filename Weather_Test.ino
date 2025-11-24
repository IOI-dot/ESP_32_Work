#include <WiFi.h>
#include "esp_wpa2.h"   
#include "esp_wifi.h"
#include "esp_err.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid     = "AUC-WiFi";
const char* username = "OmarElHussein";     
const char* password = "Hank05041321567890*"; 

// WeatherAPI
const String apiKey  = "8b04f782bc364003b8d102854252311";  
const String query   = "30.0192,31.5029";      //AUC Bus Gate

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  // WPA2 Enterprise setup
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t*)username, strlen(username));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t*)username, strlen(username));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t*)password, strlen(password));
  esp_wifi_sta_wpa2_ent_enable();

  WiFi.begin(ssid);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "https://api.weatherapi.com/v1/current.json?key=" +
                 apiKey + "&q=" + query;

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String payload = http.getString();

      StaticJsonDocument<2048> doc;
      deserializeJson(doc, payload);

      float precip = doc["current"]["precip_mm"];         
      int code     = doc["current"]["condition"]["code"]; 

      Serial.print("Precipitation (mm): ");
      Serial.println(precip);

      Serial.print("Condition Code: ");
      Serial.println(code);

      if (precip > 0.0) {
        Serial.println("RAIN");
      } else {
        Serial.println("NO RAIN");
      }
    } else {
      Serial.print("Error fetching WeatherAPI data. HTTP code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected");
    WiFi.begin(ssid);
  }

  delay(5000);  
}
