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
Topic JetracerControl("jetracer/manual/cmd_vel", 50);

// Defining flex sensors
FlexSensor toggleTrack(7, 3150);

// Gesture Control
GestureControl gestureControl(bno_IMU);

// User button
UserButton toggleButton(10);

void setup() {
    Serial.begin(115200);

    // Initializing classes
    wifi.init();
    mqtt.init();
    bno_IMU.init();
    toggleButton.init();
    toggleTrack.init();

    // LED initialization
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
}

void loop() {
    // Call poll() regularly to allow the library to send MQTT keep alive which
    // avoids being disconnected by the broker
    mqtt.getMqttClient()->poll();

    if (toggleButton.toggle()) {
        // Running imu and gesture control, only if button is toggled
        bno_IMU.run();
        gestureControl.virtualJoystick(toggleTrack.read());

        // Turning on green light
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);

        mqtt.publishJSON(JetracerControl, gestureControl.getThrottle(), gestureControl.getSteering());
    } else {
        // Turning on red light
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);

        mqtt.publishJSON(JetracerControl, 0, 0);
    }

    // Publishing position values

}
