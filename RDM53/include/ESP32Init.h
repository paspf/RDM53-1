/*
 * This file contains header informations for "ESP32Init.cpp"
 * for the ESP32
 * 
 * Date: 2019 05 24
 * Author: Pascal Pfeiffer
 */

#ifndef RDM53INIT
    #define RDM53INIT
    #include "mainUSR.h"
    #include <Arduino.h>
    #include <WiFi.h>
    #include "time.h"
    #include <rom/rtc.h>
    #include <WiFiClientSecure.h>
    #include "esp_wpa2.h"
    #include <ESPmDNS.h>
    #include <WiFiUdp.h>
    #include <ArduinoOTA.h>
    #include "piezo.h"

    #define GMT_OFFSET_SEC 3600
    #define DAYLIGHT_OFFSET_SEC 3600
    //const char *ntpServerIs = "pool.ntp.org";
    //const long  gmtOffset_sec = 3600;
    //const int   daylightOffset_sec = 3600;

    extern PiezoInterface piezo;
    void RDMWiFiInit();
    void setUpHomeWiFi();
    void setUpTHMWifi();
    int printLocalTime();
    void OTAirInit();
    uint8_t getBatteryPercentage();
#endif
