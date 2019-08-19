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
    if (tcs.begin(0x29, &Wire1)) {
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
    int m = millis();
    float red, green, blue;
    tcs.setInterrupt(false);  // turn on LED

    tcs.getRGB(&red, &green, &blue);

    tcs.setInterrupt(true);  // turn off LED
    
    this->red = int(red);
    this->green = int(green);
    this->blue = int(blue);

    #ifdef colTrack_DEBUG
        Serial.println("-----colTrack_DEBUG-----");
        Serial.print("R:\t"); Serial.print(this->red); 
        Serial.print("\tG:\t"); Serial.print(this->green); 
        Serial.print("\tB:\t"); Serial.print(this->blue);
        Serial.println();
        Serial.println("-----colTrack_DEBUG_END-----");
    #endif
    Serial.print("Color Tracking time: ");
    Serial.println(millis() - m);
}

/*
 * get the recent sensor color as a value useable for lineTracking
 * 0 - black (playground)
 * 1 - floor (floor) 1600
 * 2 - yellow (enemy goal)
 * 3 - green (our goal)
 * -1 - Error
 * RANGES:
 * floor:
 * R: 90 - 102
 * G: 78 - 91
 * B: 51 - 68
 * yellow:
 * R: 108 - 113
 * G: 89 - 95
 * B: 34 - 39
 * green:
 * R: 80 - 93
 * G: 105 - 113
 * B: 45 - 56
 * black:
 * R: 85 - 109
 * G: 51 - 102
 * B: 51 - 85
 */
short ColTrack::getLTcolor() {
    // yellow
    if( (red >= 106 && red <= 113) &&
        (green >= 89 && green <= 95) &&
        (blue >= 34 && blue <= 39)
    ) {
        #ifdef colTrack_DEBUG
            Serial.println("ColTrack: yellow");
        #endif
        return 2;
    }
    // green
    if( (red >= 80 && red <= 93) &&
        (green >= 103 && green <= 115) &&
        (blue >= 45 && blue <= 60)
    ) {
        #ifdef colTrack_DEBUG
            Serial.println("ColTrack: green");
        #endif
        return 2;
    }
    // black
    if( (red >= 85 && red <= 109) &&
        (green >= 51 && green <= 102) &&
        (blue >= 51 && blue <= 85)
    ) {
        #ifdef colTrack_DEBUG
            Serial.println("ColTrack: black");
        #endif
        return 0;
    }
    // floor
    if( (red >= 90 && red <= 102) &&
        (green >= 78 && green <= 91) &&
        (blue >= 51 && blue <= 68)
    ) {
        #ifdef colTrack_DEBUG
            Serial.println("ColTrack: floor");
        #endif
        return 1;
    }
    #ifdef colTrack_DEBUG
        Serial.println("ColTrack: Error");
    #endif        
    return -1;
}
