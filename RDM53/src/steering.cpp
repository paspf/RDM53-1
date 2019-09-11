/* This file contains the steering functions
 * using the EnginesInterface
 * 
 * setVal is the function which has to be called to change where the car drives to.
 *
 * Creation Date: 12.06.2019
 * Author: Jan Kühnemund
 */
#include <steering.h>
#ifndef RDM_MAIN
    #include <EnginesInterface.h>
    extern EnginesInterface enginesInt;
    extern Location mylocation;
    #include "connectivity.h"
#endif

//#define DEBUG_STEERING

/**
 * valType: 0 = speed, 1 = turnrate
 * valType = 0: value 0-255 speed forward, 256-511 speed backward
 * speed 0x00 00 -> backwards max speed
 * speed 0x00 FF -> backwards stop
 * speed 0x01 00 -> forwards stop
 * speed 0x01 FF -> forwards max speed
 * valType = 1: value 0-127 left, 128 straight, 129-255 right
 */
void SteeringInterface::setVal(bool valType, int value)
{
    if(valType == 0){ //Speed is set
        if(value > 0xFF){//forwards
            dirGen = 0;
            speedValNow = (0xFF & value);
                        #ifdef DEBUG_STEERING
            Serial.print("SpeedValNow setVal forwards: ");
            Serial.println(speedValNow);
            #endif        
        }
        else{//backwards
            //Serial.println("if 1 else");
            dirGen = 1;
            speedValNow = 0xFF - (0xFF & value);
            #ifdef DEBUG_STEERING
            Serial.print("SpeedValNow setVal backwards: ");
            Serial.println(speedValNow);
            #endif
        }
    }
    else if(valType == 1){
        turnValGiven = (value & 0xFF);
        #ifdef DEBUG_STEERING
        Serial.print("turnValGiven: ");
        Serial.println(turnValGiven);
        #endif
    }
    valUpdate = true;
    //sendString("Inside Steering.setVal");
}

/**
 * setPilot has to be called periodically so that the engine Values are set correctly.
 * setPilot does not accept values. It uses values in the steering object.
 */
int SteeringInterface::setPilot()
{
    // check if a val update has occured
    if(valUpdate == false) {
        return -1;
        #ifdef DEBUG_STEERING
        sendString("Set Pilot: no new value");
        #endif
    }
    dirLeft = dirGen;
    dirRight = dirGen;

    if(turnValGiven == 0x00)
    {
        turnValGiven++;
    }

    if(turnValGiven > 0x89) //Right
    {
        double turnPercent = (double(0xFF & turnValGiven)-128.0) /128.0;
        #ifdef DEBUG_STEERING
        Serial.print("turnPercentRight: ");
        Serial.println(turnPercent);
        #endif
        
        if ((dirGen == 0 && speedValNow < 64) || (dirGen == 1 && speedValNow > 191)){
            if(turnPercent > 0.5) 
            {
                dirRight = !dirGen;   
                enginesRight = speedValNow * (turnPercent - 0.5);
            }
            else {
                enginesRight = speedValNow * (1.0 - turnPercent * 2);       
            }
            enginesLeft = speedValNow;
        }
        else
        {
            enginesRight = turnPercent * 0xFF;
            enginesLeft = turnPercent * 0xFF;
            dirRight = !dirGen;
        }
        
        
    }

    else if(turnValGiven < 0x76) //Left
    {        
        double turnPercent = 1.0 -(double(0xFF & turnValGiven) / 128.0);
        #ifdef DEBUG_STEERING
        Serial.print("turnPercentLeft: ");
        Serial.println(turnPercent);
        #endif
        
        if ((dirGen == 0 && speedValNow < 64) || (dirGen == 1 && speedValNow > 191)){
            if(turnPercent > 0.5) 
            {
                dirLeft = !dirGen;    
                enginesLeft = speedValNow * (turnPercent - 0.5);
            }
            else {
                enginesRight = speedValNow * (1.0 - turnPercent * 2);       
            }   
            enginesRight = speedValNow;
        }
        else
        {
            enginesRight = turnPercent * 0xFF;
            enginesLeft = turnPercent * 0xFF;
            dirLeft = !dirGen;
        }
        
    }
    else
    {
        enginesLeft = speedValNow;
        enginesRight = speedValNow;
    }
    #ifdef DEBUG_STEERING
    Serial.print("EL: ");
    Serial.println(enginesLeft);
    Serial.print("ER: ");
    Serial.println(enginesRight);
    #endif
    staticEngines();

    // actual value is now set, no further update required
    valUpdate = false;
    return 0;
}

void SteeringInterface::staticEngines(){
    enginesInt.setEFL(dirLeft, enginesLeft);
    enginesInt.setEBL(dirLeft, enginesLeft);
    enginesInt.setEFR(dirRight, enginesRight);
    enginesInt.setEBR(dirRight, enginesRight);
}

/**
 * Function which controls the speed. Based on the PID Controller
 */
float SteeringInterface::speedControl(float targetSpeed, float currentSpeed){
    e = targetSpeed - currentSpeed;
    Ta_Speed = millis() - lastCalc;
    lastCalc = millis();

    esum = esum + e;
    float correctedSpeed = Kp_Speed * e + Ki_Speed * Ta_Speed * esum + Kd_Speed * (e - ealt)/Ta_Speed;
    if (correctedSpeed < 0){
        correctedSpeed *= -1;
    }
    if (correctedSpeed > 255){
        correctedSpeed = 255;
    }
    ealt = e;
    return correctedSpeed;
}

/**
 * This function sets the target speeds of each side.
 * It also calls speedControl to set the engineValues and checks the direction.
 * When done it calls static Engines to send Vals to Engines.
 */
void SteeringInterface::navigation(float currentHeading, float currentLeftSpeed, float currentRightSpeed){
    float targetLeftSpeed, targetRightSpeed;
    if (curveRadius != 0  && (degreeToTurnTo > currentHeading + maxDeviation || degreeToTurnTo < currentHeading - maxDeviation))
    {    
        targetLeftSpeed = (curveRadius + carWidth / 2) * (targetGenSpeed / curveRadius);
        targetRightSpeed = (curveRadius - carWidth / 2) * (targetGenSpeed / curveRadius);
    }
    else
    {
        if (degreeToTurnTo > currentHeading + maxDeviation){
            targetLeftSpeed = targetGenSpeed;
            targetRightSpeed = -targetGenSpeed;
        }
        else if (degreeToTurnTo < currentHeading - maxDeviation){
            targetLeftSpeed = -targetGenSpeed;
            targetRightSpeed = targetGenSpeed;
        }
        else
        {
            targetLeftSpeed = targetGenSpeed;
            targetRightSpeed = targetGenSpeed;
        }     
    }
    enginesLeft = speedControl(targetLeftSpeed, currentLeftSpeed);
    enginesRight = speedControl(targetRightSpeed, currentRightSpeed);
    if(targetLeftSpeed < 0){
        dirLeft = 1;
    }
    else dirLeft = 0;
    if(targetRightSpeed < 0) {
        dirRight = 1;
    }
    else dirRight = 0;
    staticEngines();
}
/**
 * This function sets the speed the device should have and by how many degrees it should turn 
 * (negative counterclockwise, positive clockwise)
 * If the amount to turn is larger than 180 it will take the shorter route. 
 */
void SteeringInterface::setNavigation(float speed, float degrees){
    degreeToTurnTo = mylocation.getHeading() + degrees;
    targetGenSpeed = speed;
    curveRadius = 0;
    //turnToDegrees(degreeToTurnTo);
}
/**
 * This function sets the speed the device should have and by how many degrees it should turn 
 * (negative counterclockwise, positive clockwise)
 * If the amount to turn is larger than 180 it will take the shorter route. 
 * It also sets a curve radius.
 */
void SteeringInterface::setNavigation(float speed, float degrees, float curveRadius){
    degreeToTurnTo = mylocation.getHeading() + degrees;
    targetGenSpeed = speed;
    if (degrees > 0){
        curveRadius *= -1;
    }
    this->curveRadius = curveRadius;
    //turnToDegrees(degreeToTurnTo);
}

void SteeringInterface::straightForewards(int speed) {
    speed += 0x100;
    setVal(1,128);
    setVal(0,speed);
}

void SteeringInterface::straightBackwards(int speed) {
    speed = 0x100 - speed;
    setVal(1,128);
    setVal(0,speed);
}

bool SteeringInterface::directionIsBackwards() {
    if(dirLeft == 0 && dirRight == 0)
        return false;
    return true;
    
}

bool SteeringInterface::directionIsForewards() {
    if(dirLeft == 1 || dirRight == 1)
        return false;
    return true;
}