#include <Arduino.h>
#include <WiFi.h>
#include "esp_wpa2.h"
#include <ArduinoMqttClient.h>


// Sett inn dine variabler:
static const char* EAP_SSID     = "eduroam";
static const char* EAP_IDENTITY = "hermagr@stud.ntnu.no";
static const char* EAP_USERNAME = "hermagr";
static const char* EAP_PASSWORD = "***";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "129.241.30.177";
int port = 1883;




void setup() {
    Serial.begin(115200);
    delay(500);

    // Tøm eventuelle gamle tilkoblinger
    WiFi.disconnect(true);

    // Sett WiFi til stasjonsmodus
    WiFi.mode(WIFI_STA);

    // Oppgi identitet, brukernavn og passord for WPA2 Enterprise
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t*)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t*)EAP_USERNAME, strlen(EAP_USERNAME));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t*)EAP_PASSWORD, strlen(EAP_PASSWORD));

    // (Om skolen krever CA-sertifikat, bruk f.eks. esp_wifi_sta_wpa2_ent_set_ca_cert() her)
    //esp_wifi_sta_wpa2_ent_set_ca_cert()

    // Aktiver WPA2 Enterprise uten egen config-struct:
    esp_err_t err = esp_wifi_sta_wpa2_ent_enable();
    if (err != ESP_OK) {
        Serial.print("Feil ved esp_wifi_sta_wpa2_ent_enable(): ");
        Serial.println(err);
    }

    // Koble til SSID
    WiFi.begin(EAP_SSID);

    Serial.printf("Kobler til WPA2 Enterprise-nettverket %s ...\n", EAP_SSID);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        wl_status_t st = WiFi.status();
        Serial.printf(" WiFi.status() = %d\n", st);
    }

    Serial.println("\nTilkoblet!");
    Serial.print("Min IP-adresse: ");
    Serial.println(WiFi.localIP());

    Serial.println("");
    Serial.print("Attempting to connect to the MQTT broker: ");
    Serial.println(broker);

    mqttClient.setId("MyESP32Client");
    mqttClient.setUsernamePassword("ais2104", "ais2104");
    if (!mqttClient.connect(broker, port)) {
        Serial.print("MQTT error: ");
        Serial.println(mqttClient.connectError());
    }


    Serial.println("Currently connected to the MQTT broker!");
    Serial.println();
}

void loop() {
    // Her kan du fortsette med f.eks. MQTT
}
