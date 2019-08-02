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

/*
* Line Tracker Interface
* 
*/
class lineTrackInterface {
    private:
    int pin;
    // color lowValv highVal
    /*
    * Best sensors: FL, BR
    * white yellow and green are not consistant
    */
    short black[2] = {3800,4095};
    short white[2] = {500,3799};
    short yellow[2] = {1200,1300};
    short green[2] = {1100,1199};
    int rawSensorValue;
    int colorCode;
    
    public:
    lineTrackInterface(int pinConst);
    int calibrateSensor(int bl0, int bl1, int wh0, int wh1, int ye0, int ye1, int gr0, int gr1);
    void readSensor();
    int getRawSensorValue();
    int getColorCode();
};

#endif