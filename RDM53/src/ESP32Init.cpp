/*
 * This file contains all Init functions
 * for the ESP32
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 */

#include "main.h"
#include <Arduino.h>
#include <WiFi.h>

class ESP32Init {
    public:
    /*
    * Initialize WiFi for the ESP 32
    */
    void RDMWiFiInit() {
    WiFi.begin(RDM_SSID, RDM_SSID_PW);
    Serial.println("\n\rConnecting Wifi...");
    int i = 0;
    while(WiFi.status() != WL_CONNECTED && i < 20) {                        // try to connect with WiFi network
        delay(300);
        i++;
    }
    if( i >= 20) {
        Serial.println("Unable to connect to WiFi Network, WiFi disabled!");
        WiFi.disconnect(true);                                                // reconnect to WiFi Network disabled
        WiFi.mode(WIFI_OFF);                                                  // turn off WiFi SW
        yield();                                                              // run background tasks
    }
    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected");
        Serial.printf("Singal strength:");
        Serial.println(WiFi.RSSI());
        Serial.printf("IP address: ");
        Serial.println(WiFi.localIP());

        Serial.printf("Client MAC address: ");
        Serial.println(WiFi.macAddress());

        Serial.print("Host MAC address: ");
        Serial.println(WiFi.BSSIDstr());
        Serial.println("-----------------------");
        }
    }
};