/*
 * This file contains the heading files for the lidar interface
 * using the Adafruit VL53L0X libary
 * 
 * Date: 2019 06 10
 * Author: Pascal Pfeiffer
 */
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
    // objects for the vl53l0x
    Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
    Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
    void setID();
    public:
    // this holds the measurement
    VL53L0X_RangingMeasurementData_t measure1;
    VL53L0X_RangingMeasurementData_t measure2;
    void expanderWrite(int, uint8_t);
    void initLox();
    int readLOXSensors();
};

