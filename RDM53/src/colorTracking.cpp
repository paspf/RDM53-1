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

/*
 * Constructor for colTrack
 * initialize TCS34725
 */
colTrack::colTrack() {
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
void colTrack::readSensor() {
    float red, green, blue;
    
    tcs.setInterrupt(false);  // turn on LED

    tcs.getRGB(&red, &green, &blue);


    tcs.setInterrupt(true);  // turn off LED
    
    #ifdef colTrack_DEBUG
    Serial.print("R:\t"); Serial.print(int(red)); 
    Serial.print("\tG:\t"); Serial.print(int(green)); 
    Serial.print("\tB:\t"); Serial.print(int(blue));
    #endif
}

/*
 * get the recent sensor color as a value useable for lineTracking
 */
short colTrack::getLTcolor() {
    return 0;
}