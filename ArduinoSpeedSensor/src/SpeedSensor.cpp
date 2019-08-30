#include "SpeedSensor.h"

int holesLeftDisk = 20;
int holesRightDisk = 20;
long prevTimeLeft = 0;
long prevTimeRight = 0;
/**
 * Function that returns rotational speed of left Disk by dividing the rotations by time passed
 */
float doMeasureLeft(){
  float s = measure(holesLeftDisk, prevTimeLeft);
  prevTimeLeft = micros();
  return s;
}
/**
 * Function that returns rotational speed of right Disk by dividing the rotations by time passed
 */
float doMeasureRight(){
  float s = measure(holesRightDisk, prevTimeRight);
  prevTimeRight = micros();
  return s;

}
float measure(int holes,long prevTime){
  long timePassed = micros() - prevTime;
  if(timePassed > 1000000 || timePassed < 500){
      return 0;
  }
  return (float) holes * ((float) timePassed / 1000000.0);
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