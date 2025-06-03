//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#include "Connections/MQTT.hpp"

MQTT* MQTT::instance = nullptr;

// Constructor for MQTT class
MQTT::MQTT(WiFiClient* wiFiClient, const char* broker, int port, bool trigger, GestureControl &gestureControl) : broker_(broker), port_(port), lastMillis(millis()), gestureControl(gestureControl) {
    mqttClient = new MqttClient(wiFiClient);
    instance   = this;
    for (int i = 0; i < 6; i++) jointAngles[i] = 0.0;
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
    if (topicStr == "mom/invalidPose") {
        String message = "";

        // Read exactly messageSize bytes from the client
        for (int i = 0; i < messageSize; i++) {
            char c = (char)mqttClient->read();
            message += c;
        }

        if (message == "TRUE") {
            digitalWrite(LED_BLUE, LOW);
        } else if (message == "FALSE") {
            digitalWrite(LED_BLUE, HIGH);
        }
    }

    if (topicStr == "mom/joint0") {
        jointAngles[0] = mqttClient->read();
    } else if (topicStr == "mom/joint1") {
        jointAngles[1] = mqttClient->read();
    } else if (topicStr == "mom/joint2") {
        jointAngles[2] = mqttClient->read();
    } else if (topicStr == "mom/joint3") {
        jointAngles[3] = mqttClient->read();
    } else if (topicStr == "mom/joint4") {
        jointAngles[4] = mqttClient->read();
    } else if (topicStr == "mom/joint5") {
        jointAngles[5] = mqttClient->read();
    }

}

double* MQTT::getJointAngles() {
    return jointAngles;
}
