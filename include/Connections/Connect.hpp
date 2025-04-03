//
// Created by Herman Hårstad Gran on 03/04/2025.
//

#ifndef MOM_GLOVE_CONNECT_HPP
#define MOM_GLOVE_CONNECT_HPP

#include <WiFi.h>
#include <Arduino.h>
#include <ArduinoMqttClient.h>

class Connect{
public:
    Connect();

private:
    WiFiClient* wiFiClient;
};

#endif //MOM_GLOVE_CONNECT_HPP
