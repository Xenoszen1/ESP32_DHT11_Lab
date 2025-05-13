#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// Replace with your network credentials
const char* ssid = "IXORA_eduroam";
const char* password = "ixorafrozn21";

// Replace with your Google Apps Script Web App URL
const char* scriptURL = "https://script.google.com/macros/s/AKfycbzIRRQqDXAFcJMHQs_1rh0sfO_dxM2Vmc-2VcT2WcEHaRB_YJv54hcm7PEFn_gvGgc/exec";

// DHT setup
#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  dht.begin();
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptURL);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      Serial.print("Data sent. Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending data. HTTP response code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

// Add to your existing ESP32 code

// Data validation function
bool validateSensorReading(float reading) {
  // Example for temperature sensor - adjust for your sensor type
  if (isnan(reading)) {
    return false;
  }
  
  // Check if reading is within reasonable range
  if (reading < -40.0 || reading > 80.0) {
    return false;
  }
  
  return true;
}

// Update your existing data sending function
void sendDataToGoogleSheets(float sensorValue) {
  // Validate reading before sending
  if (!validateSensorReading(sensorValue)) {
    Serial.println("Invalid sensor reading detected. Skipping.");
    return;
  }
  
  // Existing code for sending data to Google Sheets...
}