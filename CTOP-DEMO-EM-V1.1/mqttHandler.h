#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <WiFi.h>
#include <PubSubClient.h>
// MQTT Configuration
const char* mqtt_server = "10.2.16.116";
const int mqtt_port = 1883;
const char* topic = "oneM2M/req/vendor"; 


WiFiClient espClient;
PubSubClient client(espClient);


void connectMQTT() {
    client.setServer(mqtt_server, mqtt_port);

    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);

        if (client.connect(clientId.c_str())) {
            Serial.println("Connected to MQTT Broker");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Trying again in 5 seconds...");
            delay(5000);
        }
    }
}


void sendMQTTMessage(float voltage, float current, float frequency, float power, float energy, float pressure) {
    String payload = "{";
    payload += "\"Authentication\": \"Bearer 3b094d5ac9a3b4390fde013382b16147\",";
    payload += "\"token_id\": \"1\",";
    payload += "\"data\": {";
    payload += "\"voltage\": " + String(voltage) + ",";
    payload += "\"current\": " + String(current) + ",";
    payload += "\"frequency\": " + String(frequency) + ",";
    payload += "\"power\": " + String(power) + ",";
    payload += "\"energy\": " + String(energy) + ",";
    payload += "\"pressure\": " + String(pressure);
    payload += "}}";  // Close JSON properly

    char msg[256];  // Buffer for MQTT message
    payload.toCharArray(msg, sizeof(msg));

    Serial.print("Publishing message: ");
    Serial.println(msg);

    client.publish(topic, msg);  // Assuming `client` and `topic` are defined globally
}

#endif