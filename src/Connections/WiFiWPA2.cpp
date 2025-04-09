//
// Created by Herman Hårstad Gran on 02/04/2025.
//
#include "Connections/WiFiWPA2.hpp"

WiFiWPA2::WiFiWPA2() {
    wiFiClient = new WiFiClient;

}

void WiFiWPA2::init() {
    // Removing old connections
    WiFi.disconnect(true);

    // Setting to Station mode
    WiFi.mode(WIFI_STA);

    // Identity, name, and password for Wi-Fi school security
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t*)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t*)EAP_USERNAME, strlen(EAP_USERNAME));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t*)EAP_PASSWORD, strlen(EAP_PASSWORD));

    // Activating WPA2
    esp_err_t err = esp_wifi_sta_wpa2_ent_enable();
    if (err != ESP_OK) {
        Serial.print("Wrong with esp_wifi_sta_wpa2_ent_enable(): ");
        Serial.println(err);
    }

    // Connect to WiFi SSID
    WiFi.begin(EAP_SSID);

    Serial.printf("Connecting to WPA2 Enterprise-network %s ...\n", EAP_SSID);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        wl_status_t st = WiFi.status();
        Serial.printf(" WiFi.status() = %d\n", st);
    }

    Serial.println("Connected!");
    Serial.printf("My IP-address: ");
    Serial.println(WiFi.localIP());

}

void WiFiWPA2::homeInit() {
    WiFi.disconnect(true);

    WiFi.begin(ssid, password);

    Serial.printf("Connecting to home Wi-Fi-network %s ...\n", ssid);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        wl_status_t st = WiFi.status();
        Serial.printf(" WiFi.status() = %d\n", st);
    }

    Serial.println("Connected!");
    Serial.printf("My IP-address: ");
    Serial.println(WiFi.localIP());

}

WiFiClient* WiFiWPA2::getWiFiClient() {
    return wiFiClient;
}