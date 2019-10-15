/*
 * This file contains header informations for "ESP32Init.cpp"
 * for the ESP32
 * 
 * Creation date: 2019 05 24
 * Author: Pascal Pfeiffer
 */

#ifndef RDM53INIT_H
#define RDM53INIT_H

// includes
#include <Arduino.h>
#include <ArduinoOTA.h>

// defines
#define GMT_OFFSET_SEC 3600
#define DAYLIGHT_OFFSET_SEC 3600

// functions
void RDMWiFiInit();
void setUpHomeWiFi();
void setUpTHMWifi();
int printLocalTime();
void OTAirInit();
uint8_t getBatteryPercentage();

#endif
