//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#ifndef MOM_GLOVE_MQTT_HPP
#define MOM_GLOVE_MQTT_HPP

#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include "Connections/Topic.hpp"

class MQTT{
public:
    MQTT(WiFiClient* wiFiClient, const char* broker, int port);

    void init();

    void send(Topic& topic, float value);

    MqttClient* getMqttClient();

private:
    MqttClient* mqttClient;
    const char* broker_;
    int port_;
    unsigned long lastMillis;
};

#endif //MOM_GLOVE_MQTT_HPP
