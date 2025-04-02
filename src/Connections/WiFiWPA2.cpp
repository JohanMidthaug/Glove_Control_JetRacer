//
// Created by Herman Hårstad Gran on 02/04/2025.
//
#include "Connections/WiFiWPA2.hpp"

WiFiWPA2::WiFiWPA2() {
    wiFiClient = new WiFiClient;

    EAP_SSID = "eduroam";
    EAP_IDENTITY = "hermagr@stud.ntnu.no";
    EAP_USERNAME = "hermagr";
    EAP_PASSWORD = "";
}

void WiFiWPA2::init() {
    // Removing old connections
    WiFi.disconnect(true);

    // Setting to Station mode
    WiFi.mode(WIFI_STA);

    // Identity, name, and password for wifi school security
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
        Serial.print(".");
        wl_status_t st = WiFi.status();
        Serial.printf(" WiFi.status() = %d\n", st);
    }

    Serial.println("Connected!");
    Serial.printf("My IP-address: ");
    Serial.println(WiFi.localIP());

}