/*
 * This file contains the heading files for the lidar interface
 * using the Adafruit VL53L0X libary
 * 
 * Creation date: 2019 06 10
 * Author: Pascal Pfeiffer
 */

#ifndef LIDAR_H
#define LIDAR_H

#include "Adafruit_VL53L0X.h"

// port expander I2C adress
#define EXP_ADDRESS 0x20

/*
* Port expander pinning:
* P0 -> lidar back             0x35 LOX5 measureLidar[5]
* P1 -> lidar right            0x34 LOX4 measureLidar[4]
* P2 -> front left down        0x32 LOX1 measureLidar[1]
* P3 -> Links left             0x36 LOX6 measureLidar[6]
* P4 -> lidar front left up    0x30 LOX0 measureLidar[0]
* P5 -> lidar front right down 0x31 LOX2 measureLidar[2]
* P6 -> lidar front right up   0x33 LOX3 measureLidar[3]
*/
// sensor I2C adresses
#define LOX0_ADDRESS 0x30
#define LOX1_ADDRESS 0x31
#define LOX2_ADDRESS 0x32
#define LOX3_ADDRESS 0x33
#define LOX4_ADDRESS 0x34
#define LOX5_ADDRESS 0x35
#define LOX6_ADDRESS 0x36

// Assign lox to port expander
#define SHT_LOX0 0x10
#define SHT_LOX1 0x04
#define SHT_LOX2 0x20
#define SHT_LOX3 0x40
#define SHT_LOX4 0x02
#define SHT_LOX5 0x01
#define SHT_LOX6 0x08

class lidar {
    private:
    bool isInit = false;
    // objects for the vl53l0x (Lidar sensors)
    Adafruit_VL53L0X lox0 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox5 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox6 = Adafruit_VL53L0X();
    void setID();
    public:
    // this holds the measurements
    VL53L0X_RangingMeasurementData_t measureLidar[7];
    void expanderWrite(int, uint8_t);
    void initLox();
    void readLOXSensors();
    int printLOXValues();
};

#endif