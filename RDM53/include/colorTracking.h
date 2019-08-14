/*
 * This file contains all heading informations
 * used to detect the color of the line
 * the line itself is detected in lineTracking.cpp
 * 
 * Date: 2019 08 14
 * Author: Pascal Pfeiffer
 */

#ifndef colorTracking_H
#define colorTracking_H

class colTrack {
    private:
    Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
    public:
    colTrack();
    short getLTcolor();
    void readSensor();
};

#endif