#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>  // Make sure to import library for dht sensors

// Add your network credentials
const char* ssid = "***";
const char* password = "***";

// Set up server URL
const char* serverName = "https://***/api/device-events/log";

// Set your license key
const char* licenseKey = "***";

#define DHTPIN 4          // DHT sensor data pin
#define DHTTYPE DHT11     // DHT11 or DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
}

void loop() {
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;

    http.begin(serverName);

    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"license\": \"" + String(licenseKey) + "\", \"data\": \"{\\\"temperature\\\":\\\"" + String(temperature) + "\\\", \\\"message\\\":\\\"Temperature reading: " + String(temperature) + "°C\\\"}\"}";
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    }
    else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
  else {
    Serial.println("Error in WiFi connection");
  }

  delay(15000);
}
