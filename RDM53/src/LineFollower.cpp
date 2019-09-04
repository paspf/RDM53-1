
#include "steering.h"
#include "lineTracking.h"
#include "LineFollower.h"

extern SteeringInterface steering;
extern lineTrackInterface lineSensorFrontLeft;
extern lineTrackInterface lineSensorFrontRight;
extern lineTrackInterface lineSensorBackLeft;
extern lineTrackInterface lineSensorBackRight;



void LineFollower::checkMod() {
    
    // Modus1, falls RDM rechts vorne auf schwarzen Streifen kommt, nach links fahren für 0,5 Sekunden
    if(mod == 1) {
        steering.setVal(0,0x010);
        steering.setVal(1,255);
        if((millis() - startMod1 > 500) && (lineSensorFrontRight.getColorCode() != 0)) {
            mod = 0;
            steering.straightForewards(0xA0);
        }
    
    }
    // Modus2, falls RDM links vorne auf schwarzen Streifen kommt, nach rechts fahren dür 0,5 Sekunden
    if(mod == 2) {
        steering.setVal(0,0x010);
        steering.setVal(1,0);
        if((millis() - startMod2 > 500) && (lineSensorFrontLeft.getColorCode() != 0)){
            mod = 0;
            steering.straightForewards(0xA0);
        }
    }

}
 
void LineFollower::followLine(){



    // all line sensors are read in 
    int rawValueFL = lineSensorFrontLeft.getColorCode();
    int rawValueFR = lineSensorFrontRight.getColorCode();
    int rawValueBL = lineSensorBackLeft.getColorCode();
    int rawValueBR = lineSensorBackRight.getColorCode();

    
    // rechst vorne erkennt schwarzen Streifen
    if(rawValueFR  == 0 &&
        rawValueFL != 0 &&
        rawValueBL != 0 &&
        rawValueBR != 0 && 
        mod == 0) {
            
            mod = 1;
            startMod1 = millis();
    }

    // links vorne erkennt schwarzen Streifen
    if(rawValueFL  == 0 &&
        rawValueFR != 0 &&
        rawValueBL != 0 &&
        rawValueBR != 0 && 
        mod == 0) {
            
            mod = 2;
            startMod2 = millis();
    }

    // normaler Modus --> geradeaus Fahren
    if(mod == 0) {
        steering.straightForewards(0xA0);
    }

    checkMod();
    /*
    // if the front sensors are in the black stripe, the car drive back and turn  
    if (rawValueFL == 0 && rawValueFR == 0){
        steering.setVal(1,128);
        steering.setVal(1,80);
        steering.setVal(0,0x0000);
        return;
    }
    // if no sensor is in the black stripe, the car drive straightforward
    if (rawValueBL != 0 && rawValueFL != 0 && rawValueBR != 0 && rawValueFR != 0) {
        steering.setVal(1,128);
        steering.setVal(0,0x0175);     
        return;
    }
    
    // if the left front sensor is in the stripe,
    // turn the car in the position that both left sensors are in stripe
    if(rawValueFL == 0 && rawValueBL != 0){
        steering.setVal(1,255);
        return;
    }
    
    // if both left sensors are in the stripe, stop the car 
    if(rawValueFL == 0 && rawValueBL == 0) {
        steering.setVal(1,128);
        return;
    }
    
    if(rawValueFL != 0 && rawValueBL == 0 && cntleft == 0) {
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

    // if the right front sensor is in the stripe,
    //turn the car in the position that both right sensors are in stripe
    if(rawValueFR == 0 && rawValueBR != 0) {
        steering.setVal(1,0);
        return;
    }
    // if both right sensors are in the stripe, stop the car
    if(rawValueFR == 0 && rawValueBR == 0) {
        steering.setVal(1,128);
        return;
    }


    if(rawValueFR != 0 && rawValueBR == 0 && cntright == 0) {
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
    */
    
}