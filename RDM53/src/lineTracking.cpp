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


int lineTrackInterface::calibrateSensor(int bl0, int bl1, int wh0, int wh1, int ye0, 
    int ye1, int gr0, int gr1) {
        black[0] = bl0;
        black[1] = bl1;
        white[0] = wh0;
        white[1] = wh1;
        yellow[0] = ye0;
        yellow[1] = ye1;
        green[0] = gr0;
        green[1] = gr1;
    }
/*
 * Read out sensor value and calculate color
 * 0 - black (playground) 2800 - 2900
 * 2 - white (floor) 1600
 * 3 - Gelb (enemy goal) 1200 - 1300
 * 4 - dark green (our goal) 1100 - 1199
 * -1 - Error
*/
int lineTrackInterface::getColorCode() {
    int rawValue = analogRead(pin);
    if(rawValue >= black[0] || rawValue <= black[1]) {
        return 0;
    }
    else if(rawValue >= white[0] || rawValue <= white[1]) {
        return 1;
    }
    else if(rawValue >= yellow[0] || rawValue <= yellow[1]) {
        return 2;
    }
    else if(rawValue >= green[0] || rawValue <= green[1]) {
        return 3;
    }
    return -1;
}
