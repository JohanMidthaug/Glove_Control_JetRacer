#include <Arduino.h>
#include <Connections/MQTT.hpp>
#include <Connections/WiFiWPA2.hpp>

// Creating WIFI class
WiFiWPA2 wifi;

// Creating MQTT class
MQTT mqtt(wifi.getWiFiClient(), "129.241.30.177", 1883);

// Creating topics: Topic | Update Interval
Topic xValue("mom/xValue", 1000);
Topic yValue("mom/yValue", 1000);
Topic zValue("mom/zValue", 1000);

void setup() {
    Serial.begin(115200);

    // Initializing classes
    wifi.init();
    mqtt.init();
}

void loop() {
    // Call poll() regularly to allow the library to send MQTT keep alive which
    // avoids being disconnected by the broker
    mqtt.getMqttClient()->poll();

    // Sending message: Topic | Value
    mqtt.send(xValue, 0.2);
    mqtt.send(yValue, 0.2);
    mqtt.send(zValue, 0.2);

}
