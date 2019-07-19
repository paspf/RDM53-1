/*
 * This file contains the lidar interface
 * using the Adafruit VL53L0X libary
 * 
 * Pin 21: SDA
 * Pin 22: SCL
 * both pins need pull-up resistors with 2K49 Ohm (Tested on testboard)
 * 
 * Date: 2019 06 10
 * Author: Pascal Pfeiffer
 */

#include "lidar.h"
#include <Wire.h>

/*
 * Expander write is a simple function, that
 * puts out the byte date to the device with 
 * the i2c address
 */
void lidar::expanderWrite(int i2caddr, byte data)
{
  Wire.beginTransmission(i2caddr);
  Wire.write(data);
  Wire.endTransmission();   
}

/*
 * Initilaize all Lidar sensors
 */
void lidar::initLox() {
  //Serial.println("VL53L0X init");

  // reset all lox sensors
  expanderWrite(EXP_ADDRESS, 0x00);
  delay(10);
  // all unreset
  expanderWrite(EXP_ADDRESS, 0x7F);
  delay(10);

  setID();
}

/*
 * Set a custom I2C ID on all lidar sensors
 * uses the port expander to enable and disable the sensors
 */
void lidar::setID() {
  isInit = false;
  
  // activating LOX1 and reseting other LOX
  expanderWrite(EXP_ADDRESS, SHT_LOX1);
  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    expanderWrite(EXP_ADDRESS, B11000001);
    return;
  }
  delay(10);
 
  // activating LOX2
  expanderWrite(EXP_ADDRESS, SHT_LOX1 | SHT_LOX2);
  //delay(10);

  // initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    return;
  }

  // activating LOX3
  expanderWrite(EXP_ADDRESS, SHT_LOX1 | SHT_LOX2 | SHT_LOX3);
  //delay(10);

  // initing LOX3
  if(!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot third VL53L0X"));
    return;
  }

  // activating LOX4
  expanderWrite(EXP_ADDRESS, SHT_LOX1 | SHT_LOX2 | SHT_LOX3 | SHT_LOX4);
  //delay(10);

  // initing LOX4
  if(!lox4.begin(LOX4_ADDRESS)) {
    Serial.println(F("Failed to boot fourth VL53L0X"));
    return;
  }

  // activating LOX5
  expanderWrite(EXP_ADDRESS, SHT_LOX1 | SHT_LOX2 | SHT_LOX3 | SHT_LOX4 | SHT_LOX5);
  //delay(10);

  // initing LOX4
  if(!lox5.begin(LOX5_ADDRESS)) {
    Serial.println(F("Failed to boot fifth VL53L0X"));
    return;
  }

  // activate all
  // expanderWrite(EXP_ADDRESS, 0x7F);

  isInit = true;
}

/*
 * readLOXSensors()
 * Reads out the data of all Lox sensors
 * built in the RDM53
 */
int lidar::readLOXSensors() {
  if(isInit == false) {
    Serial.println("Not all Lidar Sensor initialized!");
    return 1;
  }
  lox1.rangingTest(&measureLidar1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measureLidar2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measureLidar3, false); // pass in 'true' to get debug data printout!
  lox4.rangingTest(&measureLidar4, false); // pass in 'true' to get debug data printout!
  lox4.rangingTest(&measureLidar5, false); // pass in 'true' to get debug data printout!
  return 0;
}

/*
 * readLOXSensors()
 * Print out lidar distances
 * for debugging purposes
 */
int lidar::printLOXValues() {
  if(isInit == false) {
    Serial.println("Not all Lidar Sensor initialized!");
    return 1;
  }
  
    // print sensor one reading
  Serial.print("1: ");
  if(measureLidar1.RangeStatus != 4) {     // if not out of range
    Serial.print(measureLidar1.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }

  // print sensor two reading
  Serial.print(" 2: ");
  if(measureLidar2.RangeStatus != 4) {
    Serial.print(measureLidar2.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }

  // print sensor three reading
  Serial.print(" 3: ");
  if(measureLidar3.RangeStatus != 4) {
    Serial.print(measureLidar3.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }

  // print sensor four reading
  Serial.print(" 4: ");
  if(measureLidar4.RangeStatus != 4) {
    Serial.print(measureLidar4.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  
  // print sensor five reading
  Serial.print(" 5: ");
  if(measureLidar5.RangeStatus != 4) {
    Serial.print(measureLidar5.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }

  Serial.println();
  return 0;
}