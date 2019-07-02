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
            //Serial.println("if 1");
            dirLeft = 0;
            dirRight = 0;
            dirGen = 0;
            speedValNow = (0xFF & value);
        }
        else{//backwards
            //Serial.println("if 1 else");
            dirLeft = 1;
            dirRight = 1;
            dirGen = 1;
            speedValNow = 0xFF - (0xFF & value);
        }
    }
    else if(valType == 1){
        turnValGiven = (value & 0xFF);
    }
    /*
    Serial.print("speedValNow set to: ");
    Serial.println(speedValNow);
    Serial.print("turnValGiven set to:");
    Serial.println(turnValGiven);
    */
}
/*
 * setPilot has to be called periodically so that the engine Values are set correctly.
 * setPilot does not accept values. It uses values in the steering object.
 * 
 * Status 02.07.2019: RDM can turn. However it either turns in a circle or it does so very slowly.
 * 
 */

void SteeringInterface::setPilot()
{
    dirLeft = dirGen;
    dirRight = dirGen;

    if(turnValGiven == 0x00)
    {
        turnValGiven++;
    }

    if(turnValGiven > 0x80)
    {
        double turnPercent = 1.0-(double(0xFF & turnValGiven)-128.0) /128.0;
        Serial.print("turnPercentRight: ");
        Serial.println(turnPercent);
        if (dirRight == false)
        {
            dirRight = true;
        }
        else if (dirRight == true)
        {
            dirRight = false;
        } 
        enginesRight = speedValNow * turnPercent;       
    }

    else if(turnValGiven < 0x80)
    {        
        double turnPercent = -(double(0xFF & turnValGiven) / 128.0) +1.0 ;
        Serial.print("turnPercentLeft: ");
        Serial.println(turnPercent);        

        if (dirLeft == false)
            {
                dirLeft = true;
            }
        else if (dirLeft == true)
            {
                dirLeft = false;
            }
        enginesLeft = speedValNow * turnPercent;
        enginesRight = speedValNow;

    }

    else
    {
        enginesLeft = speedValNow;
        enginesRight = speedValNow;
        dirLeft = dirGen;
        dirRight = dirGen;
    }
    staticEngines();
    
}

void SteeringInterface::starter()
{
    startTime = millis();
    if(dirLeft == 0 && dirRight == 0){
        enginesInt.setEFL(0, 255);
        enginesInt.setEFR(0, 255);
        enginesInt.setEFL(0, 255);
        enginesInt.setEFL(0, 255);
    }
    else if(dirLeft == 1 && dirRight == 1){
        enginesInt.setEFL(1, 255);
        enginesInt.setEFR(1, 255);
        enginesInt.setEFL(1, 255);
        enginesInt.setEFL(1, 255);      
    }
}

void SteeringInterface::staticEngines(){
    enginesInt.setEFL(dirLeft, enginesLeft);
    enginesInt.setEBL(dirLeft, enginesLeft);
    enginesInt.setEFR(dirRight, enginesRight);
    enginesInt.setEBR(dirRight, enginesRight);
}