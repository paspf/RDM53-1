#include <Arduino.h>
#include <Wire.h>

#include "SpeedSensor.h"

char c[5];

float rotSpeedLeft;
float rotSpeedRight;
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
    Serial.println("receiveEvent");
    int i = 0;
    while (0 < Wire.available()) {
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
  Serial.println("requestEvent");
  // respond to the question
  if (c[0] == 0xA) {
    converter.number = (rotSpeedLeft + rotSpeedRight) /2;

    Wire.write(converter.buffer, 4);
    Serial.println(converter.number);
  }

    if (c[0] == 0xB) {
    converter.number = rotSpeedLeft;

    Wire.write(converter.buffer, 4);
    Serial.println(converter.number);
  }

    if (c[0] == 0xC) {
    converter.number = rotSpeedRight;

    Wire.write(converter.buffer, 4);
    Serial.println(converter.number);
  }
}
/**
 * Command which gets called by Interrupt on left Wheel. 
 * Calls doCountLeft to update rotational Speed.
 */
void leftPassed(){
  rotSpeedLeft = doCountLeft();
}
/**
 * Command which gets called by Interrupt on right Wheel. 
 * Calls doCountRight to update rotational Speed.
 */
void rightPassed(){
  rotSpeedRight = doCountRight();
}
/**
 * Setup Function
 */
void setup() {

    Wire.begin(0x70);
    Wire.onRequest(requestEvent); // data request to slave
    Wire.onReceive(receiveEvent); // data slave recieved 
    Serial.begin(115200);

    attachInterrupt(0,leftPassed,RISING);
    attachInterrupt(1,rightPassed,RISING);
}
/**
 * Loop Function
 */
void loop() {
  // put your main code here, to run repeatedly:
}
