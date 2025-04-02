//
// Created by Herman Hårstad Gran on 02/04/2025.
//

#ifndef MOM_GLOVE_WIFI_HPP
#define MOM_GLOVE_WIFI_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wpa2.h>

class WiFiWPA2{
public:
    WiFiWPA2();

    void init();

private:
    WiFiClient *wiFiClient;
    static const char* EAP_SSID;
    static const char* EAP_IDENTITY;
    static const char* EAP_USERNAME;
    static const char* EAP_PASSWORD;
};

#endif //MOM_GLOVE_WIFI_HPP
