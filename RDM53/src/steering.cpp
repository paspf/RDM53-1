/* This file contains the steering functions
 * using the EnginesInterface
 *
 * Creation Date: 12.06.2019
 * Last worked on:
 * By:
 * Author: Jan Kühnemund
 */

#include <steering.h>
#include <EnginesInterface.h>

EnginesInterface engines;

void staticSteering(){}

void dynamicSteering(){}

int setSpeed(bool dir, int setSpeed){
    if(dir == 0){
        int currentSpeed = setSpeed;
        return currentSpeed;
    }
    return 0;
}

void setTurn(int currentSpeed){

}

void getMoving(bool dir,int currentSpeed)
{
    if(currentSpeed < 255 * 0.8 && dir == 0){
        engines.setEFL(0, 255);
        engines.setEFR(0, 255);
        engines.setEFL(0, 255);
        engines.setEFL(0, 255);
    }
    else if(currentSpeed < 255 * 0.8 && dir == 1){
        engines.setEFL(1, 255);
        engines.setEFR(1, 255);
        engines.setEFL(1, 255);
        engines.setEFL(1, 255);        
    }
}