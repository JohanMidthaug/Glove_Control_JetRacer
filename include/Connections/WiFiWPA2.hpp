//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#ifndef MOM_GLOVE_WIFI_HPP
#define MOM_GLOVE_WIFI_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wpa2.h>
#include <secret/Secrets.hpp>

/**
 * Class for connecting the ESP32 to wifi vie WPA2 connection
 * Needed for school ID verification
 */
class WiFiWPA2{
public:
    /**
     * Constructor for this class
     */
    WiFiWPA2();

    /**
     * Initializing the class, used in void setup
     */
    void init();

    void homeInit();

    /**
     * Getter function for pointer to wificlient
     * @return pointer to wificlient
     */
    WiFiClient* getWiFiClient();

private:
    WiFiClient *wiFiClient;
};

#endif //MOM_GLOVE_WIFI_HPP
