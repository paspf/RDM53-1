/*
 * This file contains the line tracking interface
 * using the TCRT5000 and ESP32 analog in
 * 
 * Date: 2019 06 10
 * Author: Pascal Pfeiffer
 */

#include "lineTracking.h"

// #define LINE_TRACKING_DEBUG

/*
 * parametrized constructor
 */
lineTrackInterface::lineTrackInterface(int pinConst) {
    pin = pinConst;
    } 

/*
 * Read out the raw sensor value
*/
void lineTrackInterface::readSensor() {
    rawSensorValue = analogRead(pin);
    #ifdef LINE_TRACKING_DEBUG
    Serial.print("read Sensor:");
    Serial.println(rawSensorValue);
    #endif
}

/*
 * get the latest raw sensor value
 */
int lineTrackInterface::getRawSensorValue() {
    return rawSensorValue;
}

/*
 * Calibrate the sensor colors
 */
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
        return 0;
    }

/*
 * Read out sensor value and calculate color
 * 0 - black (playground) 2800 - 2900
 * 1 - white (floor) 1600
 * 2 - yellow (enemy goal) 1200 - 1300
 * 3 - dark green (our goal) 1100 - 1199
 * -1 - Error
*/
int lineTrackInterface::getColorCode() {
    if(rawSensorValue >= black[0] && rawSensorValue <= black[1]) {
        // Serial.println("black!");
        return 0;
    }
    else if(rawSensorValue >= white[0] && rawSensorValue <= white[1]) {
        // Serial.println("white!");
        return 1;
    }
    /*
    else if(rawSensorValue >= yellow[0] && rawSensorValue <= yellow[1]) {
        // Serial.println("yellow!");
        return 2;
    }
    else if(rawSensorValue >= green[0] && rawSensorValue <= green[1]) {
        // Serial.println("green!");
        return 3;
    }
    // Serial.println("err!");
    */
    return -1;
}
