//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#ifndef MOM_GLOVE_MQTT_HPP
#define MOM_GLOVE_MQTT_HPP

#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include "Connections/Topic.hpp"
/**
 * Class for creating MQTT Client for publishing
 */
class MQTT{
public:
    /**
     * Constructor for MQTT class
     * @param wiFiClient Needs an instance of wifiClient to work
     * @param broker Broker IP address
     * @param port Broker port
     */
    MQTT(WiFiClient* wiFiClient, const char* broker, int port);

    /**
     * Function for initializing the class, used in void setup
     */
    void init();

    /**
     * Function for publishing to broker
     * @param topic Topic name
     * @param value Value to send
     */
    void send(Topic& topic, double value);

    /**
     * Getter function for pointer to mqttclient
     * @return
     */
    MqttClient* getMqttClient();

private:
    MqttClient* mqttClient;
    const char* broker_;
    int port_;
    unsigned long lastMillis;
};

#endif //MOM_GLOVE_MQTT_HPP
