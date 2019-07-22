/*
 * This file contains the heading files for the lidar interface
 * using the Adafruit VL53L0X libary
 * 
 * Date: 2019 06 10
 * Author: Pascal Pfeiffer
 */

#ifndef LIDAR_H
#define LIDAR_H
#include "Adafruit_VL53L0X.h"

// port expander I2C adress
#define EXP_ADDRESS 0x20

// sensor I2C adresses
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32
#define LOX4_ADDRESS 0x33
#define LOX5_ADDRESS 0x34
#define LOX6_ADDRESS 0x35
#define LOX7_ADDRESS 0x36

// Assignment to port expander
#define SHT_LOX1 0x1
#define SHT_LOX2 0x2
#define SHT_LOX3 0x4
#define SHT_LOX4 0x8
#define SHT_LOX5 0x10
#define SHT_LOX6 0x20
#define SHT_LOX7 0x40

class lidar{
    private:
    bool isInit = false;
    // objects for the vl53l0x (Lidar sensors)
    Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox5 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox6 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox7 = Adafruit_VL53L0X();
    void setID();
    public:
    // this holds the measurements
    VL53L0X_RangingMeasurementData_t measureLidar1; // L1 in schematic
    VL53L0X_RangingMeasurementData_t measureLidar2; // L2 in schematic
    VL53L0X_RangingMeasurementData_t measureLidar3; // L3 in schematic
    VL53L0X_RangingMeasurementData_t measureLidar4; // L4 in schematic
    VL53L0X_RangingMeasurementData_t measureLidar5; // L5 in schematic
    VL53L0X_RangingMeasurementData_t measureLidar6; // L6 in schematic
    VL53L0X_RangingMeasurementData_t measureLidar7; // L7 in schematic
    void expanderWrite(int, uint8_t);
    void initLox();
    int readLOXSensors();
    int printLOXValues();
};

#endif