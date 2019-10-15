/*
 * This file contains all heading informations
 * used to detect the color of the line
 * the line itself is detected in lineTracking.cpp
 * 
 * Creation date: 2019 08 14
 * Author: Pascal Pfeiffer
 */

#ifndef colorTracking_H
#define colorTracking_H

#include "Adafruit_TCS34725.h"

/**
 * This Class handles all the function for
 * the color sensor TCS34725
 */
class ColTrack {
    private:
    Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X);
    /** recent red sensor value */
    int red;
    /** recend green sensor value */
    int green;
    /** recent blue sensor value */
    int blue;
    public:
    void initColTrack();
    short getLTcolor();
    void printRawValues();
    void readSensor();
};

#endif
