/* This file includes all header information for the steering 
 * using the EnginesInterface
 *
 * Creation Date: 12.06.2019
 * Author: Jan Kühnemund
 */
#include <Arduino.h>

#define STEERING_SPEED 0
#define STEERING_TURN 1

class SteeringInterface{
    public:
    int speedValPrevious = 0;
    int speedValNow = 0; // do not drive
    int turnValGiven = 128; // do not turn
    int enginesLeft;
    int enginesRight;
    int startTime;
    bool dirLeft = false;
    bool dirRight = false;
    bool dirGen = false;
    bool valUpdate = false; // valUpdate is set, when setVal is called
    int setPilot();
    void setVal(bool, int);
    void setForward(int);
    void setBackward(int);
    void staticEngines();
};