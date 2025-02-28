#ifndef CTOPHANDLER_H
#define CTOPHANDLER_H

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

void postData(String type, float luminosity, String status) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
      
        // Update the URL to point to your API endpoint
        http.begin("https://ctop.iiit.ac.in/api/cin/create/79");
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", "Bearer 5aaef2196b77249860376082db90d62a");
        // Create a JSON document to store the data
        DynamicJsonDocument jsonDoc(1024);
        jsonDoc["type"] = String(type);  // Convert to String explicitly
        jsonDoc["luminosity"] = String(luminosity, 2);  // Convert float to String with 2 decimal places
        jsonDoc["status"] = String(status);  

        // Serialize JSON to a string
        String requestBody;
        serializeJson(jsonDoc, requestBody);
        Serial.println(requestBody);

        // Send the POST request
        int httpResponseCode = http.POST(requestBody);

        // Check the response from the server
        if (httpResponseCode == 200) {  // Success
            String response = http.getString();
            Serial.println(httpResponseCode);
            Serial.println(response);

            // Blink the LED to indicate success
            digitalWrite(LED_BUILTIN, HIGH); 
            delay(500);  // LED on for 500ms
            digitalWrite(LED_BUILTIN, LOW);  
        } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }

        http.end();  // End the HTTP connection
    } else {
        Serial.println("WiFi not connected. Cannot send data.");
    }
}

#endif // CTOPHANDLER_H
