//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#ifndef MOM_GLOVE_MQTT_HPP
#define MOM_GLOVE_MQTT_HPP

#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>

class MQTT{
public:
    MQTT(char broker, int port);

    void init();

private:
    MqttClient* mqttClient;
    const char broker_;
    const int port_;
};

#endif //MOM_GLOVE_MQTT_HPP
