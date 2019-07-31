/*
 * This file contains the functions which read all the
 * sensors and refresh sensor data
 * 
 * Date: 2019 07 30
 * Author: Pascal Pfeiffer
 */

#include "readSensors.h"
#include "lineTracking.h"
#include "lidar.h"

extern lineTrackInterface lineSensorFrontLeft;
extern lineTrackInterface lineSensorFrontRight;
extern lineTrackInterface lineSensorBackLeft;
extern lineTrackInterface lineSensorBackRight;
extern lidar lidarSensors;

/*
 * This function reads out all Line Tracking and Ultrasonic Sensors
 * and stores the values in the sensor objects
 */
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

/*
 * This function reads out all Lidar Sensors
 * and stores the values in the sensor objects
 * 
 * FreeRTOS
 * Execution Priority : 2
 * Execution Core : 0
 */
void lidarloop(void * pvParameters) {
    while(true) {
        Serial.println("Read all Lidar Sensors:");
        //lidarSensors.readLOXSensors();
        delay(10);
    }
}