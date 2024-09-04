#include <WiFi.h>
#include <HTTPClient.h>

// Add your network credentials
const char* ssid = "***";
const char* password = "***";

// Set up server URL
const char* serverName = "https://***/api/device-events/log";

// Set your license key
const char* licenseKey = "***";

#define REED_PIN 4  // Reed sensor pin

int lastState = HIGH;

void setup() {
  Serial.begin(115200);

  pinMode(REED_PIN, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
}

void loop() {
  int currentState = digitalRead(REED_PIN);

  if (currentState != lastState) {
    String state = (currentState == LOW) ? "Closed" : "Opened";
    sendStateToServer(state);
    lastState = currentState;
  }
}

void sendStateToServer(String state) {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"license\": \"" + String(licenseKey) + "\", \"data\": \"{\\\"state\\\":\\\"" + state + "\\\", \\\"message\\\":\\\"Reed sensor state: " + state + "\\\"}\"}";

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
}
