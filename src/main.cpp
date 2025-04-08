#include <Arduino.h>
#include <Connections/MQTT.hpp>
#include <Connections/WiFiWPA2.hpp>
#include <Connections/IMU.hpp>

// Creating WI-FI class
WiFiWPA2 wifi;

// Creating IMU class
IMU bno_IMU;

// Creating MQTT class
MQTT mqtt(wifi.getWiFiClient(), "129.241.30.177", 1883);

// Creating topics: Topic | Update Interval (ms)
/*
Topic xValue("mom/xValue", 1000);
Topic yValue("mom/yValue", 1000);
Topic zValue("mom/zValue", 1000);
*/

Topic heading("mom/heading", 100);
Topic pitch("mom/pitch", 100);
Topic roll("mom/roll", 100);

void setup() {
    Serial.begin(115200);

    // Initializing classes
    wifi.init();
    mqtt.init();
    bno_IMU.init();
}

void loop() {
    // Call poll() regularly to allow the library to send MQTT keep alive which
    // avoids being disconnected by the broker
    mqtt.getMqttClient()->poll();

    // Sending message: Topic | Value
    mqtt.send(heading, bno_IMU.heading());
    mqtt.send(pitch, bno_IMU.pitch());
    mqtt.send(roll, bno_IMU.roll());

}
