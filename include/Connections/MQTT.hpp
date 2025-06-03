//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#ifndef MOM_GLOVE_MQTT_HPP
#define MOM_GLOVE_MQTT_HPP

#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include "Connections/Topic.hpp"
#include "Control/GestureControl.hpp"

/*!
@startuml
class MQTT {
  +MQTT(WiFiClient* wiFiClient, const char* broker, int port)
  +init()
  +send(topic: Topic&, value: double)
  +getMqttClient(): MqttClient*
  -mqttClient: MqttClient*
  -broker_: const char*
  -port_: int
  -lastMillis: unsigned long
  -lastValue: double
}
@enduml
 */

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
    MQTT(WiFiClient* wiFiClient, const char* broker, int port, bool trigger, GestureControl &gestureControl);

    /**
     * Function for initializing the class, used in void setup
     */
    void init();

    /**
     * Function for publishing to broker
     * @param topic Topic name
     * @param value Value to send
     */
    void publish(Topic& topic, double value);

    /**
     * Function for sending strings to broker, used for gripper boolean value
     * @param topic The topic name
     * @param string String value to send
     */
    void publishString(Topic& topic, String string);

    /**
     * Getter function for pointer to mqttclient
     * @return
     */
    MqttClient* getMqttClient();

    /**
     * Function for subscribing to a topic
     * @param topic
     */
    void subscribe(Topic& topic);

    double* getJointAngles();
private:
    MqttClient* mqttClient;
    GestureControl& gestureControl;
    const char* broker_;
    int port_;
    unsigned long lastMillis;
    double lastValue;
    static MQTT* instance;

    void messageHandler(int messageSize);
    static void messageRouter(int messageSize);
    void computeForwardKinematics(const double q[6]);
    double jointAngles[6];   // holds the latest joint0…joint5 values

};

#endif //MOM_GLOVE_MQTT_HPP
