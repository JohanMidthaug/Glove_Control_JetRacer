//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#include "Connections/MQTT.hpp"

// Constructor for MQTT class
MQTT::MQTT(WiFiClient* wiFiClient, const char* broker, int port) : broker_(broker), port_(port), lastMillis(millis()) {
    mqttClient = new MqttClient(wiFiClient);
}

// Initializing function, used in setup
void MQTT::init() {
    Serial.println("");
    Serial.print("Attempting to connect to the MQTT broker: ");
    Serial.println(broker_);

    // Connecting to NTNU broker
    mqttClient->setId("MyESP32Client");
    mqttClient->setUsernamePassword("ais2104", "ais2104");
    if (!mqttClient->connect(broker_, port_)) {
        Serial.print("MQTT error: ");
        Serial.println(mqttClient->connectError());
    }

    Serial.println("Currently connected to the MQTT broker!");
    Serial.println();
}

// Sending topic data
void MQTT::send(Topic& topic, double value) {

    if ((millis() - topic.getLastMillis() >= topic.getInterval()) && topic.update(value)) {
        topic.setLastMillis(millis());
        // Debug messages:
        Serial.print("Sending message to: ");
        Serial.println(topic.getTopic());
        Serial.println(value);
        topic.updateLastValue(value);

        // Sending topic and value
        mqttClient->beginMessage(topic.getTopic());
        mqttClient->print(value);
        mqttClient->endMessage();
    }
}

// Getter function for client
MqttClient* MQTT::getMqttClient() {
    return mqttClient;
}