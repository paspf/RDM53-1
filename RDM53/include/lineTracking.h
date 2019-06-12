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

    public:
    lineTrackInterface(int pinConst);
    int readRawSensorValue();
    int getColorCode();
};