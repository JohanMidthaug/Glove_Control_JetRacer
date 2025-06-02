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
Topic gripper("mom/gripper", 100);

// Defining flex sensors
FlexSensor toggleHeight(4, 3100);
FlexSensor toggleTrack(7, 3100);

// Gesture Control
GestureControl gestureControl(bno_IMU);

// User button
UserButton toggleButton(10);

bool toggleGripper = 0;

void setup() {
    Serial.begin(115200);

    toggleButton.init();
    toggleHeight.init();

    // Initializing classes
    wifi.init();
    mqtt.init();
    Serial.print("Initializing sensor ...");
    bno_IMU.init();
    Serial.println(" finished!");

    // Init flexsensors
    toggleTrack.init();

    // LED initialization
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
}

void loop() {
    // Call poll() regularly to allow the library to send MQTT keep alive which
    // avoids being disconnected by the broker
    mqtt.getMqttClient()->poll();

    if (toggleButton.toggle()) {
        bno_IMU.run();
        gestureControl.virtualJoystick(toggleTrack.read() && !toggleHeight.read());
        gestureControl.heightControl(toggleHeight.read() && !toggleTrack.read());
        //gestureControl.orientation(toggleOrientation.read());
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);

        toggleGripper = gestureControl.toggleGripper(toggleTrack.read(), toggleHeight.read());

    } else {
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);
    }

    // Position values
    mqtt.send(xValue, gestureControl.getX());
    mqtt.send(yValue, gestureControl.getY());
    mqtt.send(zValue, gestureControl.getZ());

    // Orientation values
    mqtt.send(rxValue, gestureControl.getRX());
    mqtt.send(ryValue, gestureControl.getRY());
    mqtt.send(rzValue, gestureControl.getRZ());

    // Gripper
    mqtt.send(gripper, toggleGripper);

}
