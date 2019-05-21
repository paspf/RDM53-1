/*
 * Main file for RDM53
 * Board: WeMos Lolin32
 * Toochain: Arduino
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 * GitTest
 */

#include "main.h"
#include "mainUSR.h"
#include <Arduino.h>
#include "ESP32Init.cpp"
/*extern "C" {
   #include "extC.cpp"
} */

void setup() {
  Serial.begin(115200);
  ESP32Init ESP32InitObj;
  ESP32InitObj.RDMWiFiInit();
}

void loop() {
  Serial.println(millis());
  delay(500);
}
