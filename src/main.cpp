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
FlexSensor track(4, 3200);
UserButton button(10);

// Gesture Control
GestureControl gestureControl(bno_IMU);

void setup() {
    Serial.begin(115200);

    button.init();

    // Initializing classes
    wifi.init();
    mqtt.init();
    bno_IMU.init();
    track.init();
}

void loop() {
    // Call poll() regularly to allow the library to send MQTT keep alive which
    // avoids being disconnected by the broker
    mqtt.getMqttClient()->poll();

    bool read;

    Serial.println(button.buttonToggle());

    if (button.buttonToggle()) {
        bno_IMU.run();
        gestureControl.track(track);
    }

    // Position values
    mqtt.send(xValue, gestureControl.getX());
    mqtt.send(yValue, gestureControl.getY());
    mqtt.send(zValue, gestureControl.getZ());

    // Orientation values
    if (read) {
        mqtt.send(rxValue, bno_IMU.roll());
        mqtt.send(ryValue, bno_IMU.pitch());
        mqtt.send(rzValue, bno_IMU.heading());
    }
}
