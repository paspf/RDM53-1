/*
 * This file contains all header informations for the line tracking interface
 * using the TCRT5000 and ESP32 analog in
 * 
 * Date: 2019 06 10
 * Author: Pascal Pfeiffer
 */

#include <Arduino.h>

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
    short black[2] = {2800,2900};
    short white[2] = {1300,1400};
    short yellow[2] = {1200,1300};
    short green[2] = {1100,1199};
    
    public:
    lineTrackInterface(int pinConst);
    int calibrateSensor(int bl0, int bl1, int wh0, int wh1, int ye0, int ye1, int gr0, int gr1);
    int readRawSensorValue();
    int getColorCode();
};