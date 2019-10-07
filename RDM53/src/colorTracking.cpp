/*
 * This file contains all functions
 * used to detect the color of the line
 * the line itself is detected in lineTracking.cpp
 * 
 * Creation date: 2019 08 14
 * Author: Pascal Pfeiffer
 */

// includes
#include "Adafruit_TCS34725.h"
#include "colorTracking.h"

// defines
//#define colTrack_DEBUG
#define PARCOUR_0 // DT Lab own colors
// #define PARCOUR_1 // RNG lab prof colors
// #define PARCOUR_2 // RNG lab own colors

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

/**
 * read out the TCS34725 sensor
 * and store the values in the class
 */
void ColTrack::readSensor() {
    #ifdef colTrack_DEBUG
    int m = millis();
    #endif
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
    
    Serial.print("Color Tracking time: ");
    Serial.println(millis() - m);
    #endif
}

void ColTrack::printRawValues() {
    Serial.println("-----colTrack_PrintRawValues-----");
    Serial.print("R:\t"); Serial.print(this->red); 
    Serial.print("\tG:\t"); Serial.print(this->green); 
    Serial.print("\tB:\t"); Serial.print(this->blue);
    Serial.println();
    Serial.println("-----colTrack_PrintRawValues_END-----");
}

/**
 * get the recent sensor color as a value useable for lineTracking
 * 0 - black (line)
 * 1 - floor (floor) 1600
 * 2 - yellow (enemy goal)
 * 3 - green (our goal)
 * -1 - Error
 */
short ColTrack::getLTcolor() {
    // yellow
    #ifdef PARCOUR_0
    if( (red >= 105 && red <= 120) &&
        (green >= 89 && green <= 104) &&
        (blue >= 40 && blue <= 55)
    ) {
        #ifdef colTrack_DEBUG
            Serial.println("ColTrack: yellow");
        #endif
        return 2;
    }
    #endif
    #ifdef PARCOUR_1
    if( (red >= 95 && red <= 104) &&
        (green >= 89 && green <= 103) &&
        (blue >= 40 && blue <= 50)
    ) {
        #ifdef colTrack_DEBUG
            Serial.println("ColTrack: yellow");
        #endif
        return 2;
    }
    #endif

    #ifdef PARCOUR_2
    if( (red >= 100 && red <= 114) &&
        (green >= 90 && green <= 100) &&
        (blue >= 32 && blue <= 45)
    ) {
        #ifdef colTrack_DEBUG
            Serial.println("ColTrack: yellow");
        #endif
        return 2;
    }
    #endif

    // green
    #ifdef PARCOUR_0
    if( (red >= 90 && red <= 105) &&
        (green >= 105 && green <= 130) &&
        (blue >= 50 && blue <= 80)
    ) {
        #ifdef colTrack_DEBUG
            Serial.println("ColTrack: green");
        #endif
        return 3;
    }
    #endif

    #ifdef PARCOUR_1
    if( (red >= 65 && red <= 75) &&
        (green >= 105 && green <= 130) &&
        (blue >= 45 && blue <= 60)
    ) {
        #ifdef colTrack_DEBUG
            Serial.println("ColTrack: green");
        #endif
        return 3;
    }
    #endif

    #ifdef PARCOUR_2
    if( (red >= 76 && red <= 90) &&
        (green >= 105 && green <= 120) &&
        (blue >= 45 && blue <= 60)
    ) {
        #ifdef colTrack_DEBUG
            Serial.println("ColTrack: green");
        #endif
        return 3;
    }
    #endif


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
        Serial.println("ColTrack: Error, unknown color");
    #endif        
    return -1;
}

