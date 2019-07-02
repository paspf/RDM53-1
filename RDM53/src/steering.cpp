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
 * speed 00 -> backwards stop
 * speed FF -> backwards
 * speed 1 00 -> forwards max speed
 * speed 1 FF -> forwards stop
 * valType = 1: value 0-127 left, 128 straight, 129-255 right
 */
void SteeringInterface::setVal(bool valType, int value)
{
    
    if(valType == 0){ //Speed is set
        if(value > 0xFF){//forwards
            Serial.println("if 1");
            dir = 0;
            speedValNow = (0xFF & value);
        }
        else{//backwards
            Serial.println("if 1 else");
            dir = 1;
            speedValNow = 0xFF - (0xFF & value);
        }
    }
    else if(valType == 1){
        turnValGiven = value % 0xFF;
    }
    Serial.print("speedValNow set to: ");
    Serial.println(speedValNow);
    Serial.print("turnValGiven set to:");
    Serial.println(turnValGiven);
}
/*
 * setPilot has to be called periodically so that the engine Values are set correctly.
 * setPilot does not accept values. It uses values in the steering object.
 */

void SteeringInterface::setPilot()
{
    //int turnValue = (0xFF - speedValNow) * ((turnValGiven-0x80)/0x80);
    if(speedValNow > 0x79){
        if(turnValGiven > 0x80) //right
        {
            enginesRight = speedValNow - turnValGiven;
        }
        else if(turnValGiven < 0x80) //left
        {
            enginesLeft = speedValNow - turnValGiven;
        }
        else
        {
            enginesLeft = speedValNow;
            enginesRight = speedValNow;
        }
    }
    else
    {
        enginesLeft = speedValNow;
        enginesRight = speedValNow;
    }
    staticEngines();
    
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