#include <Arduino.h>
#include <Connections/MQTT.hpp>
#include <Connections/WiFiWPA2.hpp>
#include <Connections/IMU.hpp>
#include <Connections/FlexSensor.hpp>
#include "Control/GestureControl.hpp"
#include "Connections/UserButton.hpp"

// Creating WI-FI class
WiFiWPA2 wifi;

// Creating IMU class
IMU bno_IMU;

// Creating MQTT class connecting to NTNU Broker
MQTT mqtt(wifi.getWiFiClient(), "129.241.30.177", 1883);

// Defining MQTT Topics
Topic rxValue("mom/rxValue", 100);
Topic ryValue("mom/ryValue", 100);
Topic rzValue("mom/rzValue", 100);
Topic xValue("mom/xValue", 100);
Topic yValue("mom/yValue", 100);
Topic zValue("mom/zValue", 100);

// Defining flex sensors
FlexSensor toggleTrack(4, 3350);
FlexSensor toggleOrientation(5, 3350);

// Gesture Control
GestureControl gestureControl(bno_IMU);

void setup() {
    Serial.begin(115200);

    // Initializing classes
    wifi.init();
    mqtt.init();
    Serial.print("Initializing sensor ...");
    bno_IMU.init();
    Serial.println(" finished!");

    // Init flexsensors
    toggleTrack.init();
    toggleOrientation.init();

    // LED initialization
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
}

void loop() {
    // Call poll() regularly to allow the library to send MQTT keep alive which
    // avoids being disconnected by the broker
    mqtt.getMqttClient()->poll();

    bno_IMU.run();
    gestureControl.virtualJoystick(toggleTrack);
    gestureControl.orientation(toggleOrientation);

    // Position values
    mqtt.send(xValue, gestureControl.getX());
    mqtt.send(yValue, gestureControl.getY());
    mqtt.send(zValue, gestureControl.getZ());

    // Orientation values
    mqtt.send(rxValue, gestureControl.getRX());
    mqtt.send(ryValue, gestureControl.getRY());
    mqtt.send(rzValue, gestureControl.getRZ());

}
