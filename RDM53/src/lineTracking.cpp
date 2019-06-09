/*
 * This file contains the line tracking interface
 * using the TCRT5000 and ESP32 analog in
 * 
 * Date: 2019 06 0
 * Author: Pascal Pfeiffer
 */

#include "lineTracking.h"

class lineTrackInterface {
    private:
    int pin;

    public:
    // Parameterized Constructor 
    lineTrackInterface(int pinConst) 
    { 
        pin = pinConst;
    } 

    int readRawSensorValue() {
        return analogRead(pin);
    }

};