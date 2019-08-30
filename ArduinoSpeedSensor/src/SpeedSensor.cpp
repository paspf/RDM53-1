#include "SpeedSensor.h"

float holesLeftDisk = 20;
float holesRightDisk = 20;
float prevTimeLeft;
float prevTimeRight;
/**
 * Function that returns rotational speed of left Disk by dividing the rotations by time passed
 */
float doMeasureLeft(){
    float timePassed = (float) micros() - (float) prevTimeLeft;
    prevTimeLeft = micros();
    if(timePassed > 1000000|| timePassed < 800){
        return 0.0;
    }

    return 1000000.0 / holesLeftDisk / timePassed;

}
/**
 * Function that returns rotational speed of right Disk by dividing the rotations by time passed
 */
float doMeasureRight(){
    float timePassed = (float) micros() - prevTimeRight;
    prevTimeRight = (float) micros();
    if(timePassed > 1000000 || timePassed < 800){
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