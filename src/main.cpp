#include <Arduino.h>
#include <Connections/MQTT.hpp>
#include <Connections/WiFiWPA2.hpp>

WiFiWPA2 wifi;

MQTT mqtt(wifi.getWiFiClient(), "129.241.30.177", 1883);

void setup() {
    Serial.begin(115200);

    // Initializing classes
    wifi.init();
    mqtt.init();
}

void loop() {

}
