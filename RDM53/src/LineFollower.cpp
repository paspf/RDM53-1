
#include "steering.h"
#include "lineTracking.h"
#include "LineFollower.h"

extern SteeringInterface steering;
extern lineTrackInterface lineSensorFrontLeft;
extern lineTrackInterface lineSensorFrontRight;
extern lineTrackInterface lineSensorBackLeft;
extern lineTrackInterface lineSensorBackRight;


 
void LineFollower::followLine(){

    int rawValueFL = lineSensorFrontLeft.getColorCode();
    int rawValueFR = lineSensorFrontRight.getColorCode();
    int rawValueBL = lineSensorBackLeft.getColorCode();
    int rawValueBR = lineSensorBackRight.getColorCode();
    
    // stripe is on left side, line found
    if(rawValueFL == 0 && rawValueBL != 0){
        steering.setVal(1,255);
        return;
    }

    // line front and back
    if(rawValueFL == 0 && rawValueBL == 0) {
        steering.setVal(0,100);
        return;
    }
    
    if(rawValueFL != 0 && rawValueBL == 0 && (cntleft == 0 || cntleft == NULL)) {
        directionleft = 1;
        cntleft = millis();
        steering.setVal(1,255);
        return;
    }
    else if(millis() - cntleft > 100 && directionleft == 1) {
        directionleft = 2;
        cntleft = millis();
        steering.setVal(1,0);
        return;
    }
    else if(millis() - cntleft > 200 && directionleft == 2) {
        directionleft = 3;
        cntleft = millis();
        steering.setVal(0,0);
        return;
    }

    // stripe is on right side
    if(rawValueFR == 0 && rawValueBR != 0) {
        steering.setVal(1,0);
        return;
    }
    if(rawValueFR == 0 && rawValueBR == 0) {
        steering.setVal(0,100);
        return;
    }

    if(rawValueFR != 0 && rawValueBR == 0 && (cntright == 0 || cntright == NULL)) {
        directionright = 1;
        cntright = millis();
        steering.setVal(1,0);
        return;
    }
    else if(millis() - cntright > 100 && directionright == 1) {
        directionright = 2;
        cntright = millis();
        steering.setVal(1,255);
        return;
    }
    else if(millis() - cntright > 200 && directionright == 2) {
        directionright = 3;
        cntright = millis();
        steering.setVal(0,0);
        return;
    }
}