#include "SpeedSensor.h"

float holesLeftDisk = 20;
float holesRightDisk = 20;
long prevTimeLeft = 0;
long prevTimeRight = 0;
/**
 * Function calls measure, sets prev Times and returns rot speed on left side.
 */
float doMeasureLeft(){
  float s = measure(holesLeftDisk, prevTimeLeft);
  prevTimeLeft = millis();
  return s;
}
/**
 * Function calls measure, sets prev Times and returns rot speed on right side.
 */
float doMeasureRight(){
  float s = measure(holesRightDisk, prevTimeRight);
  prevTimeRight = millis();
  return s;
/**
 * Function which calculates rotational speed over time
 * Takes amount of holes and last time measured.
 * returns speed as float.
 */
}
float measure(float holes,long prevTime){
  long timePassed = millis() - prevTime;
  if(timePassed > 1000 || timePassed < 1){
      return 0;
  }
  return (1.0 / holes) * ((float) timePassed/1000.0);
}
/**
 * Function which sets the amount of holes on the left side.
 */
void setHoleLeft(float num){
  holesLeftDisk = num;
}
/**
 * Function which sets the amount of holes on the right side.
 */
void setHoleRight(float num){
  holesRightDisk = num;
}