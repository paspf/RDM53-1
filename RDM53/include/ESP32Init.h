/*
 * This file contains header informations for "ESP32Init.cpp"
 * for the ESP32
 * 
 * Date: 2019 05 24
 * Author: Pascal Pfeiffer
 */


// #include "main.h"
#include "mainUSR.h"
// #include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include <rom/rtc.h>
// #include <WiFiClientSecure.h>
#include "esp_wpa2.h"

#define NTP_SERVER_IS "pool.ntp.org"
#define GMT_OFFSET_SEC 3600
#define DAYLIGHT_OFFSET_SEC 3600
//const char *ntpServerIs = "pool.ntp.org";
//const long  gmtOffset_sec = 3600;
//const int   daylightOffset_sec = 3600;
