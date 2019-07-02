/* This file includes all header information for the steering 
 * using the EnginesInterface
 *
 * Creation Date: 12.06.2019
 * Author: Jan Kühnemund
 */
#include <Arduino.h>



class SteeringInterface{
    public:
    int speedValPrevious;
    int speedValNow = 255; // do not drive
    int turnValGiven = 128; // do not turn
    int enginesLeft;
    int enginesRight;
    int startTime;
    bool dir;
    void setPilot();
    void setVal(bool, int);
    void staticEngines();
    void starter();
};