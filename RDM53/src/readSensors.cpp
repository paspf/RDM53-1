/*
 * This file contains the functions which read all the
 * sensors and refresh sensor data
 * 
 * Date: 2019 07 30
 * Author: Pascal Pfeiffer
 */

#include "readSensors.h"
#include "lineTracking.h"

extern lineTrackInterface lineSensorFrontLeft;
extern lineTrackInterface lineSensorFrontRight;
extern lineTrackInterface lineSensorBackLeft;
extern lineTrackInterface lineSensorBackRight;

void readSensors() {
    // Serial.println("FRONT LEFT:");
    lineSensorFrontLeft.readSensor();
    // Serial.println("FRONT RIGHT:");
    lineSensorFrontRight.readSensor();
    // Serial.println("BACK LEFT:");
    lineSensorBackLeft.readSensor();
    // Serial.println("BACK RIGHT:");
    lineSensorBackRight.readSensor();
    
}
