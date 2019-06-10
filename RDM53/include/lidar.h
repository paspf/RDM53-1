/*
 * This file contains the heading files for the lidar interface
 * using the Adafruit VL53L0X libary
 * 
 * Date: 2019 06 10
 * Author: Pascal Pfeiffer
 */

#include "Adafruit_VL53L0X.h"

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t lox1measure;
VL53L0X_RangingMeasurementData_t lox2measure2;

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// set the pins to shutdown
#define LOX1_SHT 34
#define LOX2_SHT 32

void setID();

void initLox();