#include "ObstacleAvoidance.h"
#include "steering.h"
#include "lidar.h"
#include "connectivity.h"
#include "lineTracking.h"
#include "LineFollower.h"
#include "HCSR04P.h"
#include "ObstacleAndLine.h"
#include "colorTracking.h"
#include "piezo.h"
#include "PublicStructures.h"
#include "EnginesInterface.h"

extern SteeringInterface steering;
extern lidar lidarSensors;

extern lineTrackInterface lineSensorFrontLeft;
extern lineTrackInterface lineSensorFrontRight;
extern lineTrackInterface lineSensorBackLeft;
extern lineTrackInterface lineSensorBackRight;

extern HCSR04P ultraSonic;
extern ColTrack colTrack;
extern PiezoInterface piezo;
extern EnginesInterface enginesInt;
extern deviceConfig dC;



int ObstacleAndLine::checkMod() {
    
    // Modus1, falls RDM rechts vorne auf schwarzen Streifen kommt, nach links fahren für 0,5 Sekunden
    if(mod == 1) {
        steering.setVal(0,0x010);
        steering.setVal(1,255);
        if((millis() - startMod1 > 500) && (lineSensorFrontRight.getColorCode() != 0)) {
            mod = 0;
            steering.straightForewards(0xC0);
        }
    
    }
    // Modus2, falls RDM links vorne auf schwarzen Streifen kommt, nach rechts fahren dür 0,5 Sekunden
    if(mod == 2) {
        steering.setVal(0,0x010);
        steering.setVal(1,0);
        if((millis() - startMod2 > 500) && (lineSensorFrontLeft.getColorCode() != 0)){
            mod = 0;
            steering.straightForewards(0xC0);
        }
    }

    //Hindernis mit schwarzen Steifen vorne rechts
    if(mod == 69) {
        steering.setVal(0,0x010);
        steering.setVal(1,255);
        steering.setVal(0,0);
        if((millis() - startMod69 > 1000) && (lineSensorFrontRight.getColorCode() != 0)) {
            mod = 0;
            steering.straightForewards(0xC0);
        }
    }

    //Hindernis mit schwarzen Steifen vorne links
    if(mod == 70) {
        steering.setVal(0,0x010);
        steering.setVal(1,0);
        steering.setVal(0,0);
        if((millis() - startMod70 > 1000) && (lineSensorFrontLeft.getColorCode() != 0)) {
            mod = 0;
            steering.straightForewards(0xC0);
        }
    }



    // Farbsensor erkennt Ziel und hält an nach 2 Sekunden
    if(mod == 10) {
        steering.setVal(0,0x0100);
        steering.straightForewards(0x60);
        piezo.setPiezo(400);
        if((millis() - startMod10 > 3000) && (colTrack.getLTcolor() != 3)) {
            mod = 0;    
            piezo.noSound();
            steering.setVal(0,0x0100);
            dC.mode = 0x20000;
            dC.cyclicSensorRefresh = false;
            enginesInt.stopE();
            return 10;
            
        }
    }
    return 0;
}


void ObstacleAndLine::driveThroughParcour(){
    
    int rawValueFL = lineSensorFrontLeft.getColorCode();
    int rawValueFR = lineSensorFrontRight.getColorCode();
    int rawValueBL = lineSensorBackLeft.getColorCode();
    int rawValueBR = lineSensorBackRight.getColorCode();
    
    int rawValueColorSensor = colTrack.getLTcolor();


   if(checkMod() == 10) {
       return;
   }


    //Hindernis mit schwarzen Steifen vorne rechts 
    if (rawValueFR == 0 &&
        ultraSonic.getDist() < 300 &&
        lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0   &&
        mod == 0
        ) {
            mod = 69;
            startMod69 = millis();
        }
    

    //Hindernis mit schwarzen Steifen vorne links 
    if (rawValueFL == 0 &&
        ultraSonic.getDist() < 300 &&
        lidarSensors.measureLidar[0].RangeMilliMeter > 0   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0   &&
        mod == 0
        ) {
            mod = 70;
            startMod70 = millis();
        }
    
    // rechst vorne erkennt schwarzen Streifen (LineTrackingSensor)
    if(rawValueFR  == 0 &&
        rawValueFL != 0 &&
        rawValueBL != 0 &&
        rawValueBR != 0 && 
        mod == 0) {
            
            mod = 1;
            startMod1 = millis();
    }

    // links vorne erkennt schwarzen Streifen (Linetrackingsensor)
    if(rawValueFL  == 0 &&
        rawValueFR != 0 &&
        rawValueBL != 0 &&
        rawValueBR != 0 && 
        mod == 0) {
            
            mod = 2;
            startMod2 = millis();
    }

    // Farbsensor erkennt grünen Streifen --> Ziel erkannt 
    if(rawValueColorSensor == 3 && mod == 0) {
            
            mod = 10;
            startMod10 = millis();
    }


   if(mod != 0) {
       return;
   }


    //vorwärts fahren
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0
        ) {
            steering.setVal(1,128);
            steering.setVal(0,400);
            return;
        }


    //rückwärts fahren
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 250 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 250 &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 250 &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 250 &&
        lidarSensors.measureLidar[4].RangeMilliMeter < 250 &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter < 250
        ) {
            steering.setVal(1,128);
            steering.setVal(0,0);
            return;
        }




    // Sensor 0 & 1 --> dann rechts 
    if (ultraSonic.getDist() < 300 &&
        lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0
        ) {
            steering.setVal(1,240);
            return;
        }
    // Sensor 1 & 6 --> dann rechts
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 0   && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter < 300
        ) {
            steering.setVal(1,240);
            return;
        }

    // Sensor 0 & 1 & 6 --> dann rechts     
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter < 300
        ) {
            steering.setVal(1,240);
            return;
        }
    
    
    // Sensor 0 & 1 & 3 --> dann rechts
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0
        ) {
            steering.setVal(1,240);
            return;
        }

    // Sensor 0 & 1 & 2 & 3 + 6 --> dann rechts
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 200 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter < 1500
        ) {
            return;
        }




    // Sensor 3 & 2 --> dann links 
    if (ultraSonic.getDist() < 300 &&
        lidarSensors.measureLidar[0].RangeMilliMeter > 0   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0
        ) {
            steering.setVal(1,25);
            return;
        }

    // Sensor 2 & 4 --> dann links
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 0   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[4].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0
        ) {
            steering.setVal(1,25);
            return;
        }

    // Sensor 2 & 3 & 4 --> dann links     
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 0   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0
        ) {
            steering.setVal(1,25);
            return;
        }



    // Sensor 0 & 2 & 3 --> dann links
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0
        ) {
            steering.setVal(1,25);
            return;
        }

    // Sensor 0 & 1 & 2 & 3 + 4 --> dann links
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 200 &&
        lidarSensors.measureLidar[4].RangeMilliMeter < 1500 &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0
        ) {
            steering.setVal(1,25);
            return;
        }
}

void ObstacleAndLine::initValues() {
    startMod69 = 0;
    startMod70 = 0;
    startMod10 = 0;
    startMod1 = 0;
    startMod2 = 0;
    mod = 0;
}