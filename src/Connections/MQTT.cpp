//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#include "Connections/MQTT.hpp"

MQTT::MQTT(char broker, int port) : broker_(broker), port_(port) {
    mqttClient = new MqttClient();
}

void MQTT::init() {
    Serial.println("");
    Serial.print("Attempting to connect to the MQTT broker: ");
    Serial.println(broker_);

    mqttClient.setId("MyESP32Client");
    mqttClient.setUsernamePassword("ais2104", "ais2104");
    if (!mqttClient.connect(broker_, port_)) {
        Serial.print("MQTT error: ");
        Serial.println(mqttClient.connectError());
    }

    Serial.println("Currently connected to the MQTT broker!");
    Serial.println();
}