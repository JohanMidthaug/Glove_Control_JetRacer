//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#ifndef MOM_GLOVE_WIFI_HPP
#define MOM_GLOVE_WIFI_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wpa2.h>
#include <secret/Secrets.hpp>

class WiFiWPA2{
public:
    WiFiWPA2();

    void init();

    WiFiClient* getWiFiClient();

private:
    WiFiClient *wiFiClient;
    /*
    const char* EAP_SSID;
    const char* EAP_IDENTITY;
    const char* EAP_USERNAME;
    const char* EAP_PASSWORD;
     */
};

#endif //MOM_GLOVE_WIFI_HPP
