#include "SpeedSensor.h"

float holesLeftDisk = 20;
float holesRightDisk = 20;

float prevTime;
/**
 * Function that returns rotational speed of left Disk by dividing the rotations by time passed
 */
float doMeasureLeft(){
    float timePassed = (float) micros() - (float) prevTime;
    prevTime = micros();
    if(timePassed > 1000000){
        return 0.0;
    }

    return 1000000.0 / holesLeftDisk / timePassed;

}
/**
 * Function that returns rotational speed of right Disk by dividing the rotations by time passed
 */
float doMeasureRight(){
    float timePassed = (float) micros() - prevTime;
    prevTime = (float) micros();
    if(timePassed > 1000000 || timePassed < 0){
        return 0;
    }
    return 1000000.0 / holesRightDisk / timePassed;

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