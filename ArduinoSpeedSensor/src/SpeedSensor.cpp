#include "SpeedSensor.h"

float holesLeftDisk;
float holesRightDisk;

float prevTime;
/**
 * Function that returns rotational speed of left Disk by dividing the rotations by time passed
 */
float doCountLeft(){
    float timePassed = (float) millis() - prevTime;
    prevTime = (float) millis();
    if(timePassed > 1000){
        return 0;
    }

    return 1.0 / holesLeftDisk / timePassed;

}
/**
 * Function that returns rotational speed of right Disk by dividing the rotations by time passed
 */
float doCountRight(){
    float timePassed = (float) millis() - prevTime;
    prevTime = (float) millis();
    if(timePassed > 1000){
        return 0;
    }
    return 1.0 / holesRightDisk / timePassed;

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