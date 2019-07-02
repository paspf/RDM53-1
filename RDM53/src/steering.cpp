/* This file contains the steering functions
 * using the EnginesInterface
 *
 * Creation Date: 12.06.2019
 * Author: Jan Kühnemund
 */
#include <steering.h>
#ifndef RDM_MAIN
    #include <EnginesInterface.h>
    extern EnginesInterface enginesInt;
#endif

/*
 * valType: 0 = speed, 1 = turnrate
 * valType = 0: value 0-255 speed forward, 256-511 speed backward
 * valType = 1: value 0-127 left, 128 straight, 129-255 right
 */
void SteeringInterface::setVal(bool valType, int value)
{
    
    if(valType == 0){ //Speed is set
        if(value > 0xFF){
            dir = 0;
            speedValNow = 0xFF - value % 0xFF;
        }
        else{
            dir = 1;
            speedValNow = value % 0xFF;
        }
    }
    else
    if(valType == 1){
        turnValGiven = value % 0xFF;
    }
}
/*
 * setPilot has to be called periodically so that the engine Values are set correctly.
 * setPilot does not accept values. It uses values in the steering object.
 */

void SteeringInterface::setPilot()
{
    if(speedValPrevious == 0 && speedValNow != 0){
        starter();
        speedValPrevious = speedValNow;
    }
    if (startTime < millis() - 50)
    {
        int turnValue = (0xFF - speedValNow) * ((turnValGiven-0x80)/0x80);
        int enginesLeft = speedValNow - turnValue;
        int enginesRight = speedValNow + turnValue;
        staticEngines();
    }
}

void SteeringInterface::starter()
{
    startTime = millis();
    if(dir == 0){
        enginesInt.setEFL(0, 255);
        enginesInt.setEFR(0, 255);
        enginesInt.setEFL(0, 255);
        enginesInt.setEFL(0, 255);
    }
    else if(dir == 1){
        enginesInt.setEFL(1, 255);
        enginesInt.setEFR(1, 255);
        enginesInt.setEFL(1, 255);
        enginesInt.setEFL(1, 255);      
    }
}

void SteeringInterface::staticEngines(){
    enginesInt.setEFL(dir, enginesLeft);
    enginesInt.setEBL(dir, enginesLeft);
    enginesInt.setEFR(dir, enginesRight);
    enginesInt.setEBR(dir, enginesRight);
}