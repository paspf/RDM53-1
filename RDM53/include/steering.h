/* This file includes all header information for the steering 
 * using the EnginesInterface
 *
 * Creation Date: 12.06.2019
 * Author: Jan Kühnemund
 */
#include <Arduino.h>



class SteeringInterface{
    public:
    int speedValPrevious = 0;
    int speedValNow = 0; // do not drive
    int turnValGiven = 128; // do not turn
    int enginesLeft;
    int enginesRight;
    int startTime;
    bool dir = false; // forwards
    void setPilot();
    void setVal(bool, int);
    void staticEngines();
    void starter();
};