/*
 * This file contains all header informations for the line tracking interface
 * using the TCRT5000 and ESP32 analog in
 * 
 * Date: 2019 06 10
 * Author: Pascal Pfeiffer
 */

#include <Arduino.h>

#ifndef LINE_TRACKING_H
#define LINE_TRACKING_H

#define LT_FL_PIN 36
#define LT_FR_PIN 32
#define LT_BL_PIN 34
#define LT_BR_PIN 39

#define ownColorsA2007

/*
* Line Tracker Interface
* 
*/
class lineTrackInterface {
    private:
    int pin;
    // color [0]:lowVal [1]:highVal
    #ifdef ownColorsA2007
    short black[2] = {4000,4095};
    short floor[2] = {3630,3950};
    short yellow[2] = {3500,3629};
    short green[2] = {1600,2600};
    short goalIndicator[2] = {2200, 2600};
    #endif
    int rawSensorValue;
    int colorCode;
    
    public:
    lineTrackInterface(int pinConst);
    int calibrateSensor(short bl0, short bl1, short wh0, short wh1, short ye0, short ye1, short gr0, short gr1);
    void readSensor();
    int getRawSensorValue();
    int getColorCode();
};

#endif