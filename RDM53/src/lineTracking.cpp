/*
 * This file contains the line tracking interface
 * using the TCRT5000 and ESP32 analog in
 * 
 * Date: 2019 06 10
 * Author: Pascal Pfeiffer
 */

#include "lineTracking.h"

/*
 * parametrized constructor
 */
lineTrackInterface::lineTrackInterface(int pinConst) {
    pin = pinConst;
    } 

/*
 * Read out the raw sensor value
*/
int lineTrackInterface::readRawSensorValue() {
    return analogRead(pin);
    }

/*
 * Read out sensor value and calculate color
 * 0 - white (floor) 1600
 * 1 - black (playground) 2800 - 2900
 * 2 - Gelb (enemy goal) 1200 - 1300
 * 3 - dark green (our goal) 1100 - 1199
 * 5 - blue 1800 - 1999
 * 4 - Error
*/
int lineTrackInterface::getColorCode() {
    int rawValue = analogRead(pin);
    int colCode = 4;
    if(rawValue < 200) {
        colCode = 0;
    }
    return colCode;
}
