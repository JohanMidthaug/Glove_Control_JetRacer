#include <Arduino.h>
#include <Connections/MQTT.hpp>
#include <Connections/WiFiWPA2.hpp>
#include <Connections/IMU.hpp>
#include <Connections/FlexSensor.hpp>
#include "Control/GestureControl.hpp"

// Creating WI-FI class
WiFiWPA2 wifi;

// Creating IMU class
IMU bno_IMU;

// Creating MQTT class connecting to NTNU Broker
MQTT mqtt(wifi.getWiFiClient(), "129.241.30.177", 1883);

// For Mosquitto Broker
// MQTT mqtt(wifi.getWiFiClient(), "192.168.68.62", 1883);

// Creating topics: Topic | Update Interval (ms)
/*
Topic xValue("mom/xValue", 1000);
Topic yValue("mom/yValue", 1000);
Topic zValue("mom/zValue", 1000);
*/

// Defining MQTT Topics
Topic heading("mom/heading", 100);
Topic pitch("mom/pitch", 100);
Topic roll("mom/roll", 100);
Topic xPos("mom/xPos", 100);
Topic yPos("mom/yPos", 100);
Topic zPos("mom/zPos", 100);

// Defining flex sensors
FlexSensor track(4, 3100);

// Gesture Control
GestureControl gestureControl(bno_IMU);

void setup() {
    Serial.begin(115200);

    // Initializing classes
    // wifi.init();
    // wifi.homeInit();
    // mqtt.init();
    bno_IMU.init();
    track.init();
}

float totRoll = 0.0, totPitch = 0.0, totYaw = 0.0;

void loop() {
    // Call poll() regularly to allow the library to send MQTT keep alive which
    // avoids being disconnected by the broker
    /*
    mqtt.getMqttClient()->poll();

    // Sending message: Topic | Value
    if (track.read()) {
        mqtt.send(heading, bno_IMU.heading());
        mqtt.send(pitch, bno_IMU.pitch());
        mqtt.send(roll, bno_IMU.roll());
        Serial.println();
    }
    */
    bno_IMU.run();
    gestureControl.track(track);
    Serial.print("ORIENTATION: | Heading: ");
    Serial.print(bno_IMU.heading());
    Serial.print(" | Pitch: ");
    Serial.print(bno_IMU.pitch());
    Serial.print(" | Roll: ");
    Serial.print(bno_IMU.roll());
    Serial.print(" | POSITION: | x: ");
    Serial.print(gestureControl.getX());
    Serial.print(" | y: ");
    Serial.print(gestureControl.getY());
    Serial.print(" | z: ");
    Serial.println(gestureControl.getZ());


}
