#include "ObstacleAvoidance.h"
#include "steering.h"
#include "lidar.h"
#include "connectivity.h"
#include "lineTracking.h"
#include "LineFollower.h"
#include "HCSR04P.h"
#include "ObstacleAndLine.h"

extern SteeringInterface steering;
extern lidar lidarSensors;

extern lineTrackInterface lineSensorFrontLeft;
extern lineTrackInterface lineSensorFrontRight;
extern lineTrackInterface lineSensorBackLeft;
extern lineTrackInterface lineSensorBackRight;

extern HCSR04P ultraSonic;


void ObstacleAndLine::driveThroughParcour(){
    
    int rawValueFL = lineSensorFrontLeft.getColorCode();
    int rawValueFR = lineSensorFrontRight.getColorCode();
    int rawValueBL = lineSensorBackLeft.getColorCode();
    int rawValueBR = lineSensorBackRight.getColorCode();



    // if both front sensors are in the stripe, drive back
    if(rawValueFL == 0 && rawValueFR == 0) {
        steering.setVal(0,0);
        return;
    }

    // if the left front sensor is in the stripe,
    // turn the car in the position that both left sensors are in stripe
    if(rawValueFL == 0 && rawValueBL != 0){
        steering.setVal(1,255);
        return;
    }
    
    // if both left sensors are in the stripe, drive to right 
    if(rawValueFL == 0 && rawValueBL == 0) {
        steering.setVal(1,255);
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
        steering.setVal(1,0);
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
            steering.setVal(0,375);
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