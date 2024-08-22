#include <WiFi.h>
#include <HTTPClient.h>

// Add your network credentials
const char* ssid = "***";
const char* password = "***";

// Set up server URL
const char* serverName = "https://***/api/device-events/log";

// Set your license key
const char* licenseKey = "***";

int counter = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi");
}

void loop() {
  counter = counter + 1;
  
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;

    http.begin(serverName);
    
    http.addHeader("Content-Type", "application/json");
    
    String jsonData = "{\"license\": \"" + String(licenseKey) + "\", \"data\": \"{\\\"counter\\\":\\\"" + String(counter) + "\\\"}\"}";
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
