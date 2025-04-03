//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#include "Connections/MQTT.hpp"

MQTT::MQTT(WiFiClient* wiFiClient, const char* broker, int port) : broker_(broker), port_(port), lastMillis(millis()) {
    mqttClient = new MqttClient(wiFiClient);
}

void MQTT::init() {
    Serial.println("");
    Serial.print("Attempting to connect to the MQTT broker: ");
    Serial.println(broker_);

    mqttClient->setId("MyESP32Client");
    mqttClient->setUsernamePassword("ais2104", "ais2104");
    if (!mqttClient->connect(broker_, port_)) {
        Serial.print("MQTT error: ");
        Serial.println(mqttClient->connectError());
    }

    Serial.println("Currently connected to the MQTT broker!");
    Serial.println();
}

void MQTT::send(Topic& topic, float value) {

    if (millis() - topic.getLastMillis() >= topic.getInterval()) {
        topic.setLastMillis(millis());
        // Debug messages:
        Serial.print("Sending message to: ");
        Serial.println(topic.getTopic());
        Serial.println(value);

        // Sending topic and value
        mqttClient->beginMessage(topic.getTopic());
        mqttClient->print(value);
        mqttClient->endMessage();
    }
}

MqttClient* MQTT::getMqttClient() {
    return mqttClient;
}