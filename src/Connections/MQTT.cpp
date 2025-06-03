//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#include "Connections/MQTT.hpp"

MQTT* MQTT::instance = nullptr;

// Constructor for MQTT class
MQTT::MQTT(WiFiClient* wiFiClient, const char* broker, int port) : broker_(broker), port_(port), lastMillis(millis()) {
    mqttClient = new MqttClient(wiFiClient);
    instance   = this;
}

// Initializing function, used in setup
void MQTT::init() {
    Serial.println("");
    Serial.print("Attempting to connect to the MQTT broker: ");
    Serial.println(broker_);

    // Connecting to NTNU broker
    mqttClient->setId("MyESP32Client");
    //mqttClient->setUsernamePassword("ais2104", "ais2104");
    if (!mqttClient->connect(broker_, port_)) {
        Serial.print("MQTT error: ");
        Serial.println(mqttClient->connectError());
    }

    mqttClient->onMessage(messageRouter);

    Serial.println("Currently connected to the MQTT broker!");
    Serial.println();
}

// Sending topic data
void MQTT::publish(Topic& topic, double value) {

    if ((millis() - topic.getLastMillis() >= topic.getInterval()) && topic.update(value)) {
        topic.setLastMillis(millis());
        // Debug messages:
        /*
        Serial.print("Sending message to: ");
        Serial.println(topic.getTopic());
        Serial.println(value);
         */
        topic.updateLastValue(value);

        // Sending topic and value
        mqttClient->beginMessage(topic.getTopic());
        mqttClient->print(value);
        mqttClient->endMessage();
    }
}

void MQTT::publishString(Topic &topic, String string) {
    if ((millis() - topic.getLastMillis() >= topic.getInterval()) && topic.updateString(string)) {
        topic.setLastMillis(millis());
        // Debug messages:
        /*
        Serial.print("Sending message to: ");
        Serial.println(topic.getTopic());
        Serial.println(string);
         */
        topic.updateLastString(string);

        // Sending topic and value
        mqttClient->beginMessage(topic.getTopic());
        mqttClient->print(string);
        mqttClient->endMessage();
    }
}

// Getter function for client
MqttClient* MQTT::getMqttClient() {
    return mqttClient;
}

// used in setub
void MQTT::subscribe(Topic& topic) {
    mqttClient->subscribe(topic.getTopic());
    if (!mqttClient->subscribe("mom/invalidPose")) {
        Serial.println("subscribe() failed!");
    } else {
        String message = "subscribed to" + String(topic.getTopic());
        Serial.println(message);
    }
}

// Message router, the function needed static function so this work around that
void MQTT::messageRouter(int messageSize) {
    if (instance != nullptr) {
        instance->messageHandler(messageSize);
    }
}

// Handles incoming messages
void MQTT::messageHandler(int messageSize) {
    // Remember that mqttClient is an instance pointer, so you must use instance->mqttClient
    String topicStr = mqttClient->messageTopic();
    String message = "";
    for (int i = 0; i < messageSize; i++) {
        char c = (char)mqttClient->read();
        message += c;
    }

    if (topicStr == "mom/invalidPose") {


        // Read exactly messageSize bytes from the client

        if (message == "TRUE") {
            digitalWrite(LED_BLUE, LOW);
        } else if (message == "FALSE") {
            digitalWrite(LED_BLUE, HIGH);
        }
    }

    if (topicStr == "mom/xActualValue") {
        xPos = message.toFloat();
    } else if (topicStr == "mom/yActualValue") {
        yPos = message.toFloat();
    } else if (topicStr == "mom/zActualValue") {
        zPos = message.toFloat();
    } else if (topicStr == "mom/joint5") {
        rotation = message.toFloat();
    }

}

// Getter function for joint angles that MQTT Client is subscribing to
float MQTT::getXpos() {
    return xPos;
}

float MQTT::getYpos() {
    return yPos;
}

float MQTT::getZpos() {
    return zPos;
}
float MQTT::getRotation() {
    return rotation;
}

