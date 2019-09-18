/*
 * This file contains the functions which read all the
 * sensors and refresh sensor data
 * 
 * Date: 2019 07 30
 * Author: Pascal Pfeiffer
 */

// includes
#include "readSensors.h"
#include "lineTracking.h"
#include "lidar.h"
#include "PublicStructures.h"
#include "HCSR04P.h"
#include "colorTracking.h"
#include "location.h"

// extern objects
extern lineTrackInterface lineSensorFrontLeft;
extern lineTrackInterface lineSensorFrontRight;
extern lineTrackInterface lineSensorBackLeft;
extern lineTrackInterface lineSensorBackRight;
extern lidar lidarSensors;
extern deviceConfig dC;
extern HCSR04P ultraSonic;
extern ColTrack colTrack;
extern Location mylocation;

// defines
//#define DEBUG_READ_SENSORS

/**
 * This function reads out all Line Tracking and Ultrasonic Sensors
 * and stores the values in the sensor objects
 * This function is called in the main loop and running in core 1
 */
void readSensors() {
    mylocation.updateLocationVars();
    if(dC.cyclicSensorRefresh == true) {
        #ifdef DEBUG_READ_SENSORS
        Serial.println("FRONT LEFT:");
        #endif
        lineSensorFrontLeft.readSensor();
        #ifdef DEBUG_READ_SENSORS
        Serial.println("FRONT RIGHT:");
        #endif
        lineSensorFrontRight.readSensor();
        #ifdef DEBUG_READ_SENSORS
        Serial.println("BACK LEFT:");
        #endif
        lineSensorBackLeft.readSensor();
        #ifdef DEBUG_READ_SENSORS
        Serial.println("BACK RIGHT:");
        #endif
        lineSensorBackRight.readSensor();
        ultraSonic.measureDist();
        colTrack.readSensor();
    }
}

/**
 * This function reads out all Lidar Sensors
 * and stores the values in the sensor objects
 * 
 * FreeRTOS
 * Execution Priority : 2
 * Execution Core : 0
 */
void lidarloop(void * pvParameters) {
    #ifdef DEBUG_READ_SENSORS
    int t1;
    #endif
    while(true) {
        if(dC.cyclicSensorRefresh == true) {
            #ifdef DEBUG_READ_SENSORS
            int t1 = millis();
            #endif
            lidarSensors.readLOXSensors();
            #ifdef DEBUG_READ_SENSORS
            Serial.print("Read lidar sensors runtime: ");
            Serial.println(millis() - t1);
            #endif
        }
        delay(10);
    }
}