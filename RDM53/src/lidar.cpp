/*
 * This file contains the lidar interface
 * using the Adafruit VL53L0X libary
 * 
 * Date: 2019 06 10
 * Author: Pascal Pfeiffer
 */

#include "lidar.h"

void initLox() {
  Serial.println("Adafruit VL53L0X test");

  pinMode(LOX1_SHT, OUTPUT);
  pinMode(LOX2_SHT, OUTPUT);

  // reset all lox sensors
  digitalWrite(LOX1_SHT, LOW);
  digitalWrite(LOX2_SHT, LOW);
    delay(10);
  // all unreset
  digitalWrite(LOX1_SHT, HIGH);
  digitalWrite(LOX2_SHT, HIGH);
  delay(10);

  setID();
  dC.mode = 1;
}

void setID() {
  // activating LOX1 and reseting LOX2
  digitalWrite(LOX1_SHT, HIGH);
  digitalWrite(LOX2_SHT, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(LOX2_SHT, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}
