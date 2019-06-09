/*
 * This file contains the line tracking interface
 * using the TCRT5000 and ESP32 analog in
 * 
 * Date: 2019 06 0
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
 * 0 - white (floor)
 * 1 - black (playground)
 * 2 - Gelb (enemy goal)
 * 3 - dark green (our goal)
 * 4 - Error
*/
int lineTrackInterface::getColorCode() {
    int rawValue = analogRead(pin);
}
