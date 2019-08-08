/* This file includes all header information for the steering 
 * using the EnginesInterface
 *
 * Creation Date: 12.06.2019
 * Author: Jan Kühnemund
 */

#ifndef STEERING_H
#define STEERING_H

#include <Arduino.h>
#include "location.h"

#define STEERING_SPEED 0
#define STEERING_TURN 1

class SteeringInterface{
    public:

    int maxDeviation = 5;

    int speedValNow = 0; // do not drive
    int turnValGiven = 128; // do not turn
    int enginesLeft;
    int enginesRight;
    int startTime;

    bool turnHardLeft = false;
    bool turnHardRight = false;
    float degreeToTurnTo;
    float goalSpeed;

    bool dirLeft = false;
    bool dirRight = false;
    bool dirGen = false;
    bool valUpdate = false; // valUpdate is set, when setVal is called
    
    int setPilot();
    void navigation();
    void setVal(bool, int);
    void setForward(int);
    void setBackward(int);
    void staticEngines();
    void turnHardLeftBy(float);
    void turnHardRightBy(float);
    void turnToDegrees(float);
    void turnLeftBy(float);
    void turnLeftBy(float, int);
    void turnRightBy(float);
    void turnRightBy(float, int);
};

#endif
