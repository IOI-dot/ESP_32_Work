#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "AUC-WiFi";
const char* password = "Hank05041321567890*";
const String apiKey  = "8b04f782bc364003b8d102854252311";  

const String query = "Cairo";

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
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
      Serial.println("Error fetching WeatherAPI data");
    }

    http.end();
  }

  delay(5000);  
}
