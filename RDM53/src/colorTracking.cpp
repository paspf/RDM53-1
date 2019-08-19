/*
 * This file contains all functions
 * used to detect the color of the line
 * the line itself is detected in lineTracking.cpp
 * 
 * Date: 2019 08 14
 * Author: Pascal Pfeiffer
 */

#include "Adafruit_TCS34725.h"
#include "colorTracking.h"

#define colTrack_DEBUG

/**
 * initialize TCS34725
 */
void ColTrack::initColTrack() {
    Serial.print("TCS34725...");
    if (tcs.begin()) {
        Serial.println("[OK]");
        } 
    else {
        Serial.println("[FAIL]");
    }
}

/*
 * read out the TCS34725 sensor
 */
void ColTrack::readSensor() {
    
    tcs.setInterrupt(false);  // turn on LED

    tcs.getRGB(&red, &green, &blue);

    tcs.setInterrupt(true);  // turn off LED
    
    #ifdef colTrack_DEBUG
        Serial.println("-----colTrack_DEBUG-----");
        Serial.print("R:\t"); Serial.print(int(red)); 
        Serial.print("\tG:\t"); Serial.print(int(green)); 
        Serial.print("\tB:\t"); Serial.print(int(blue));
        Serial.println();
        Serial.println("-----colTrack_DEBUG_END-----");
    #endif
}

/*
 * get the recent sensor color as a value useable for lineTracking
 */
short ColTrack::getLTcolor() {
    return 0;
}