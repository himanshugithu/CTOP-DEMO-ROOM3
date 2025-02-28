#include <WiFi.h>
#include "Adafruit_VEML7700.h"
#include "ctopHandler.h"

// Replace with your WiFi credentials
const char* ssid = "SCRC_LAB_IOT";
const char* password = "Scrciiith@123";

Adafruit_VEML7700 veml = Adafruit_VEML7700();

String lastStatus = "";  // Store last status
unsigned long lastSendTime = 0;  // Timer for periodic sending
unsigned long statusChangeTime = 0;  // Timer for status change delay
bool waitingToSend = false;  // Flag for waiting

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize VEML7700 sensor
  Serial.println("Adafruit VEML7700 Auto Lux Test");

  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found");

  // Read initial lux and set status
  float lux = veml.readLux(VEML_LUX_AUTO);
  lastStatus = (lux <= 30) ? "HIGH" : "LOW";
  lastSendTime = millis();  // Start periodic sending timer
}

void loop() {
  // Read lux value
  float lux = veml.readLux(VEML_LUX_AUTO);
  
  Serial.println("------------------------------------");
  Serial.print("Lux = "); 
  Serial.println(lux);
  
  // Determine status
  String status = (lux <= 30) ? "HIGH" : "LOW";

  // Check if status changed from LOW to HIGH
  if (lastStatus == "LOW" && status == "HIGH") {
    Serial.println("Status changed from LOW to HIGH! Starting 10s timer...");
    postData("light", lux, status);
    statusChangeTime = millis();  // Start timer
    waitingToSend = true;  // Enable flag
  }

  // If 10 seconds have passed after status change, send data
  if (waitingToSend && (millis() - statusChangeTime >= 10000)) {
    Serial.println("10 seconds passed after status change! Sending data...");
    postData("light", lux, status);
    waitingToSend = false;  // Reset flag
    lastSendTime = millis();  // Reset periodic sending timer
  }

  // Send data every 10 seconds regardless of status change
  if (millis() - lastSendTime >= 10000) {
    Serial.println("10 seconds passed! Sending periodic data...");
    postData("LUX", lux, status);
    lastSendTime = millis();  // Reset timer
  }

  lastStatus = status;  // Update last status

  delay(1000);  // Check every second
}
