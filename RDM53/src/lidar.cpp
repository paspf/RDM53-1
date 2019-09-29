/*
 * This file contains the lidar interface
 * using the Adafruit VL53L0X libary
 * 
 * Pin 21: SDA
 * Pin 22: SCL
 * both pins need pull-up resistors with 2K49 Ohm (Tested on testboard)
 * 
 * Creation date: 2019 06 10
 * Author: Pascal Pfeiffer
 */

// includes
#include "lidar.h"
#include "steering.h"
#include "connectivity.h"
#include <Wire.h>


extern SteeringInterface steering;

/**
 * Expander write is a simple function, that
 * puts out the byte date to the device with 
 * the i2c address
 * for PCF85741
 * @param i2caddr i2c address of the expander
 * @param data expander output bits
 */
void lidar::expanderWrite(int i2caddr, byte data)
{
  Wire.beginTransmission(i2caddr);
  Wire.write(data);
  Wire.endTransmission();   
}

/**
 * Initialize all Lidar sensors
 */
void lidar::initLox() {
  //Wire.begin();
  //Serial.println("VL53L0X init");

  // reset all lox sensors
  expanderWrite(EXP_ADDRESS, 0x00);
  delay(10);
  // all unreset
  expanderWrite(EXP_ADDRESS, 0x7F);
  delay(10);
  setID();
}

/**
 * Set a custom I2C ID on all lidar sensors
 * uses the port expander to enable and disable the sensors
 */
void lidar::setID() {
  isInit = false;
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
  byte lidarState = 0x0;
  expanderWrite(EXP_ADDRESS, 0x0);
  delay(10);

  // activating LOX5 and reseting other LOX
  Serial.print("booting P0: fifth VL53L0X...");
  lidarState |= SHT_LOX5;
  expanderWrite(EXP_ADDRESS, 0x1);
  // initing LOX5
  if(!lox5.begin(LOX5_ADDRESS)) {
    Serial.println("[Failed]");
    lidarState &= ~SHT_LOX5;
    // return;
  }
  else {
    Serial.println("[OK]");
  }
  delay(10);
 
  // activating LOX4
  Serial.print("booting P1: fourth VL53L0X...");
  lidarState |= SHT_LOX4;
  expanderWrite(EXP_ADDRESS, lidarState);
  // initing LOX4
  if(!lox4.begin(LOX4_ADDRESS)) {
    Serial.println("[Failed]");
    lidarState &= ~SHT_LOX4;
    // return;
  }
  else {
    Serial.println("[OK]");
  }
  delay(10);

  // activating LOX1
  Serial.print("booting P2: first VL53L0X...");
  lidarState |= SHT_LOX1;
  expanderWrite(EXP_ADDRESS, lidarState);
  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println("[Failed]");
    lidarState &= ~SHT_LOX1;
    // return;
  }
  else {
    Serial.println("[OK]");
  }
  delay(10);

  // activating LOX6
  Serial.print("booting P3: sixth VL53L0X...");
  lidarState |= SHT_LOX6;
  expanderWrite(EXP_ADDRESS, lidarState);
  // initing LOX6
  if(!lox6.begin(LOX6_ADDRESS)) {
    Serial.println("[Failed]");
    lidarState &= ~SHT_LOX6;
    // return;
  }
  else {
    Serial.println("[OK]");
  }
  delay(10);

  // activating LOX0
  Serial.print("booting P4: zero VL53L0X...");
  lidarState |= SHT_LOX0;
  expanderWrite(EXP_ADDRESS, lidarState);
  // initing LOX0
  if(!lox0.begin(LOX0_ADDRESS)) {
    Serial.println("[Failed]");
    lidarState &= ~SHT_LOX0;
    // return;
  }
  else {
    Serial.println("[OK]");
  }
  delay(10);

  // activating LOX2
  Serial.print("booting P5: second VL53L0X...");
  lidarState |= SHT_LOX2;
  expanderWrite(EXP_ADDRESS, lidarState);
  // initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println("[Failed]");
    lidarState &= ~SHT_LOX2;
    // return;
  }
  else {
    Serial.println("[OK]");
  }
  delay(10);

  // activating LOX3
  Serial.print("booting P6: third VL53L0X...");
  lidarState |= SHT_LOX3;
  expanderWrite(EXP_ADDRESS, lidarState);
  // initing LOX3
  if(!lox3.begin(LOX3_ADDRESS)) {
    Serial.println("[Failed]");
    lidarState &= ~SHT_LOX3;
    // return;
  }
  else {
    Serial.println("[OK]");
  }
  delay(10);

  // activate all
  // expanderWrite(EXP_ADDRESS, 0x7F);
  isInit = true;
}

/**
 * Reads out the data of all Lox sensors
 * built in the RDM53
 */
void lidar::readLOXSensors() {
  if(isInit == false) {
    sendStringln("Not all Lidar Sensor initialized!");
    return;
  }
  lox0.rangingTest(&measureLidar[0], false); // pass in 'true' to get debug data printout!
  lox1.rangingTest(&measureLidar[1], false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measureLidar[2], false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measureLidar[3], false); // pass in 'true' to get debug data printout!
  lox4.rangingTest(&measureLidar[4], false); // pass in 'true' to get debug data printout!
  if(steering.directionIsBackwards()) {
    lox5.rangingTest(&measureLidar[5], false); // pass in 'true' to get debug data printout!
  }
  else {
    measureLidar[5].RangeMilliMeter = 8000;
  }
  
  lox6.rangingTest(&measureLidar[6], false); // pass in 'true' to get debug data printout!

  // check out of Range:
  for(int i = 0; i < 6; i++) {
    if(measureLidar[i].RangeStatus == 4) {
      measureLidar[i].RangeMilliMeter = 8000;
    }
  }
  return;
}

/*
 * Print out lidar distances
 * for debugging purposes
 */
int lidar::printLOXValues() {
  if(isInit == false) {
    Serial.println("Not all Lidar Sensor initialized!");
    return 1;
  }
  
  // print sensor one reading (front left top)
  Serial.print("0: ");
  if(measureLidar[0].RangeStatus != 4) {     // if not out of range
    Serial.print(measureLidar[0].RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }

  // print sensor two reading (front left bottom)
  Serial.print(" 1: ");
  if(measureLidar[1].RangeStatus != 4) {
    Serial.print(measureLidar[1].RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }

  // print sensor three reading (front right top)
  Serial.print(" 2: ");
  if(measureLidar[2].RangeStatus != 4) {
    Serial.print(measureLidar[2].RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }

  // print sensor four reading (front right bottom)
  Serial.print(" 3: ");
  if(measureLidar[3].RangeStatus != 4) {
    Serial.print(measureLidar[3].RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  
  // print sensor five reading (right)
  Serial.print(" 4: ");
  if(measureLidar[4].RangeStatus != 4) {
    Serial.print(measureLidar[4].RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }

  // print sensor six reading (back)
  Serial.print(" 5: ");
  if(measureLidar[5].RangeStatus != 4) {
    Serial.print(measureLidar[5].RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }

  // print sensor seven reading (left)
  Serial.print(" 6: ");
  if(measureLidar[6].RangeStatus != 4) {
    Serial.print(measureLidar[6].RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }

  Serial.println();
  return 0;
}