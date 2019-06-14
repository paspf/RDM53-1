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
  Serial.println("VL53L0X init");

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
  // activating LOX1 and reseting other LOX
  expanderWrite(EXP_ADDRESS, SHT_LOX1);
  isInit = false;

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    return;
  }
  delay(10);

  // activating LOX2
  expanderWrite(EXP_ADDRESS, SHT_LOX1 | SHT_LOX2);
  delay(10);

  // initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    return;
  }
  isInit = true;
}

int lidar::readLOXSensors() {
  if(isInit == false) {
    Serial.println("Not all Lidar Sensor initialized!");
    return 1;
  }
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  Serial.print("1: ");
  if(measure1.RangeStatus != 4) {     // if not out of range
    Serial.print(measure1.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  
  Serial.print(" ");

  // print sensor two reading
  Serial.print("2: ");
  if(measure2.RangeStatus != 4) {
    Serial.print(measure2.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  
  Serial.println();
  return 0;
}
