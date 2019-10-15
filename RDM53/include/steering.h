/* 
 * This file includes all header information for the steering 
 * using the EnginesInterface
 *
 * Creation Date: 2019 06 12
 * Author: Jan Kühnemund
 */

#ifndef STEERING_H
#define STEERING_H

#include <Arduino.h>
#include "location.h"

#define STEERING_SPEED 0
#define STEERING_TURN 1

class SteeringInterface{
private:
    int maxDeviation = 5;
    float carWidth;

    int speedValNow = 0; // do not drive
    int turnValGiven = 128; // do not turn
    int enginesLeft;
    int enginesRight;
    int startTime;

    bool turnHardLeft = false;
    bool turnHardRight = false;
    float degreeToTurnTo;
    float targetGenSpeed;
    float curveRadius;

    //Control Variables
    float esum;             //Sum of all errors
    float e;                //error
    float ealt;             //previous error
    float Kp_Speed = 1.0;   //Proportional Amplification
    float Ki_Speed = 1.0;   //Integral Amplification
    float Ta_Speed;         //Sampling time
    float Kd_Speed = 1.0;   //Differential Amplification
    int lastCalc;           //Time of last Calculation

    bool dirLeft = false;
    bool dirRight = false;
    bool dirGen = false;
    bool valUpdate = false; // valUpdate is set, when setVal is called

public: 
    int setPilot();
    void setVal(bool, int);
    void staticEngines();
    void navigation(float, float, float);

    void setNavigation(float, float);
    void setNavigation(float, float, float);

    float speedControl(float, float);
    void straightForewards(int speed);
    void straightBackwards(int speed);

    bool directionIsBackwards();
    bool directionIsForewards();
};

#endif
