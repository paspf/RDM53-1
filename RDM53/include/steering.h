/* This file includes all header information for the steering 
 * using the EnginesInterface
 *
 * Creation Date: 12.06.2019
 * Author: Jan Kühnemund
 */
#include <Arduino.h>
#ifndef RDM_MAIN
    #include <EnginesInterface.h>
    extern EnginesInterface enginesInt;
#endif


class SteeringInterface{
    public:
    int speedValPrevious;
    int speedValNow;
    int turnValGiven;
    int enginesLeft;
    int enginesRight;
    int startTime;
    bool dir;
    void setPilot();
    void setVal(bool, int);
    void staticEngines();
    void starter();
};