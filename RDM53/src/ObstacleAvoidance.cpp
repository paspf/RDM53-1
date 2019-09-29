/*
 * The obstace avoider avoids obstacles by
 * changing the driving direction
 * ObstacleAvoidance only uses lidar sensors
 * 
 * 
 * Creation date: 2019 07 31
 * Author: Taha Tekdemir
 */

#include "ObstacleAvoidance.h"
#include "steering.h"
#include "lidar.h"
#include "connectivity.h"
#include "lineTracking.h"
#include "LineFollower.h"
#include "HCSR04P.h"

extern SteeringInterface steering;
extern lidar lidarSensors;
extern HCSR04P ultraSonic;


void ObstacleAvoidance::obstaclecircuit(){
    
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
            steering.setVal(0,440);
            return;
        }


    //rückwärts fahren, wenn vorne und seiten alles blockiert ist 
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 250 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 250 &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 250 &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 250 &&
        lidarSensors.measureLidar[4].RangeMilliMeter < 250 &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter < 250
        ) {
            steering.setVal(1,128);
            steering.setVal(0,0x0000);
            return;
        }
    
    
    // Sensor 0 --> dann rechts 
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0 
        ) {
            steering.setVal(1,240);
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


    // Sensor 3 --> dann links 
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 0   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0 
        ) {
            steering.setVal(1,25);
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