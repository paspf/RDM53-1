#include <Arduino.h>
#include <Wire.h>

#include "SpeedSensor.h"

//#define DEBUG_ARDUINO_SPEED_SENS

char c[5];

float rotSpeedLeft;
float rotSpeedRight;

unsigned long lastInterrupt;

/**
 * Union which stores the Bytes of the Float
 * Is for sending floats using I2C.
 */
union floatToBytes {
    char buffer[4];
    float number;
} converter;

/**
 * Function that gets called on receive Event on I2C Interface
 * Stores up to 5 bytes.
 * If first Byte:
 * 0x0: payload is amount of holes in left disk in float format.
 * 0x1: payload is amount of holes in right disk in float format.
 */
void receiveEvent(int howMany) {
    // remember the question: H=humidity, T=temperature
    #ifdef DEBUG_ARDUINO_SPEED_SENS
    Serial.println("receiveEvent");
    #endif
    int i = 0;
    while (Wire.available()) {
        byte x = Wire.read();
        c[i] = x;
        i++;
    }
    if (c[0] == 0x0){
      for(int j = 0; j < 4; j++){
        converter.buffer[j] = c[j+1];
      }
      setHoleLeft(converter.number);
    }
    if (c[0] == 0x1){
      for(int j = 0; j < 4; j++){
        converter.buffer[j] = c[j+1];
      }
      setHoleRight(converter.number);
    }
}
/**
 * Function that gets called on request Event on I2C interface
 */ 
void requestEvent() {
  #ifdef DEBUG_ARDUINO_SPEED_SENS
  Serial.println("requestEvent");
  #endif
  // respond to the question
  if (c[0] == 0xA) {
    #ifdef DEBUG_ARDUINO_SPEED_SENS
    Serial.println("requestEvent: Rotation General");
    #endif
    converter.number = (rotSpeedLeft + rotSpeedRight) /2.0;

    Wire.write(converter.buffer, 4);
  }

    if (c[0] == 0xB) {
    #ifdef DEBUG_ARDUINO_SPEED_SENS
    Serial.println("requestEvent: Rotation Left");
    #endif
    converter.number = rotSpeedLeft;

    Wire.write(converter.buffer, 4);
  }

    if (c[0] == 0xC) {
    #ifdef DEBUG_ARDUINO_SPEED_SENS
    Serial.println("requestEvent: Rotation Right");
    #endif    
    converter.number = rotSpeedRight;

    Wire.write(converter.buffer, 4);
  }
}
/**
 * Command which gets called by Interrupt on left Wheel. 
 * Calls doCountLeft to update rotational Speed.
 */
void leftPassed(){
  lastInterrupt = millis();
  rotSpeedLeft = doMeasureLeft();
  #ifdef DEBUG_ARDUINO_SPEED_SENS
  Serial.print("Rot/s Left:");
  Serial.println(rotSpeedLeft);
  #endif
}
/**
 * Command which gets called by Interrupt on right Wheel. 
 * Calls doCountRight to update rotational Speed.
 */
void rightPassed(){
  lastInterrupt = millis();

  rotSpeedRight = doMeasureRight();
  #ifdef DEBUG_ARDUINO_SPEED_SENS
  Serial.print("Rot/s Right");
  Serial.println(rotSpeedRight);
  #endif
}
/**
 * Setup Function
 */
void setup() {

    Wire.begin(0x70);
    Wire.onRequest(requestEvent); // data request to slave
    Wire.onReceive(receiveEvent); // data slave recieved 
    Serial.begin(115200);

    lastInterrupt = millis();

    attachInterrupt(0,leftPassed,RISING);
    attachInterrupt(1,rightPassed,RISING);
}
/**
 * Loop Function
 */
void loop() {
  if(millis() - lastInterrupt > 100) {
    rotSpeedLeft = 0.0;
    rotSpeedRight = 0.0;
  }
  // put your main code here, to run repeatedly:
}
