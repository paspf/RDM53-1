/*
 * This file contains the line tracking interface
 * using the TCRT5000 and ESP32 analog in
 * 
 * Date: 2019 06 10
 * Author: Pascal Pfeiffer
 */

// includes
#include "lineTracking.h"

// #define LINE_TRACKING_DEBUG

/**
 * initialize infra red line tracking
 * sets sensor sepcific color codes
 */
lineTrackInterface::lineTrackInterface(int pinConst) {
    pin = pinConst;
    if(pin == LT_FL_PIN) {
        #ifdef ownColorsA2007
        this->calibrateSensor(-1, -1, -1, -1, -1, -1, -1, -1);
        #endif
    }
    else if(pin == LT_FR_PIN) {
        #ifdef ownColorsA2007
        this->calibrateSensor(-1, -1, -1, -1, -1, -1, -1, -1);
        #endif
    }
    else if(pin == LT_BL_PIN) {
        #ifdef ownColorsA2007
        this->calibrateSensor(-1, -1, 3150, 3700, 1500, 1649, 1650, 1900);
        #endif
    }
    else if(pin == LT_BR_PIN) {
        #ifdef ownColorsA2007
        this->calibrateSensor(-1, -1, -1, -1, -1, -1, -1, -1);
        #endif
    }
} 

/**
 * Read out the raw sensor value
 */
void lineTrackInterface::readSensor() {
    rawSensorValue = analogRead(pin);
    #ifdef LINE_TRACKING_DEBUG
    Serial.print("read Sensor:");
    Serial.println(rawSensorValue);
    #endif
}

/**
 * get the latest raw sensor value
 */
int lineTrackInterface::getRawSensorValue() {
    return rawSensorValue;
}

/**
 * Calibrate the sensor colors
 * if -1 is set for a color, the value is not changed
 * @param bl0 lower end black
 * @param bl1 upper end black
 * @param wh0 lower end white
 * @param wh1 upper end white
 * @param ye0 lower end yellow
 * @param ye1 upper end yellow
 * @param gr0 lower end greem
 * @param gr1 upper end green
 */
int lineTrackInterface::calibrateSensor(short bl0, short bl1, short wh0, short wh1, short ye0, 
    short ye1, short gr0, short gr1) {
        if(bl0 != -1) {
            black[0] = bl0;
        }
        if(bl1 != -1) {
            black[1] = bl1;
        }
        if(wh0 != -1) {
            floor[0] = wh0;
        }
        if(wh1 != -1) {
            floor[1] = wh1;
        }
        if(ye0 != -1) {
            yellow[0] = ye0;
        }
        if(ye1 != -1) {
            yellow[1] = ye1;
        }
        if(gr0 != -1) {
            green[0] = gr0;
        }
        if(gr1 != -1) {
            green[1] = gr1;
        }
        return 0;
    }


/**
 * calculate color based on recent sensor value
 * read out new values with readSensor()
 * 0 - black (playground) 2800 - 2900
 * 1 - floor (floor) 1600
 * 2 - goal indicator 2200 - 2600
 * -1 - Error
*/
int lineTrackInterface::getColorCode() {
    // black (playground border)
    if(rawSensorValue >= black[0] && rawSensorValue <= black[1]) {
        #ifdef LINE_TRACKING_DEBUG
        Serial.println("black!");
        #endif
        return 0;
    }
    // floor (floor)
    else if(rawSensorValue >= floor[0] && rawSensorValue <= floor[1]) {
        #ifdef LINE_TRACKING_DEBUG
        Serial.println("floor!");
        #endif
        return 1;
    }
    // yellow (enemy goal), dark green (our goal) ->goal indicator
    else if(rawSensorValue >= goalIndicator[0] && rawSensorValue <= goalIndicator[1]) {
        #ifdef LINE_TRACKING_DEBUG
        Serial.println("goalIndicator!");
        #endif
        return 2;
    }
    #ifdef LINE_TRACKING_DEBUG
    Serial.println("err!");
    #endif
    return -1;
}
