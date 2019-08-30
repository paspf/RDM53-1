#include "SpeedSensor.h"

int holesLeftDisk = 20;
int holesRightDisk = 20;
long prevTimeLeft = 0;
long prevTimeRight = 0;
/**
 * Function calls measure, sets prev Times and returns rot speed on left side.
 */
float doMeasureLeft(){
  float s = measure(holesLeftDisk, prevTimeLeft);
  prevTimeLeft = micros();
  return s;
}
/**
 * Function calls measure, sets prev Times and returns rot speed on right side.
 */
float doMeasureRight(){
  float s = measure(holesRightDisk, prevTimeRight);
  prevTimeRight = micros();
  return s;
/**
 * Function which calculates rotational speed over time
 * Takes amount of holes and last time measured.
 * returns speed as float.
 */
}
float measure(int holes,long prevTime){
  long timePassed = micros() - prevTime;
  if(timePassed > 1000000 || timePassed < 500){
      return 0;
  }
  return (1/(float) holes) * (1000000.0/(float) timePassed);
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