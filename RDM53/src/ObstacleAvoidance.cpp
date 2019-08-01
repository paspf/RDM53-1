#include "ObstacleAvoidance.h"
#include "steering.h"
#include "lidar.h"
#include "connectivity.h"
#include "lineTracking.h"
#include "LineFollower.h"

extern SteeringInterface steering;
extern lidar lidarSensors;

extern lineTrackInterface lineSensorFrontLeft;
extern lineTrackInterface lineSensorFrontRight;
extern lineTrackInterface lineSensorBackLeft;
extern lineTrackInterface lineSensorBackRight;



void ObstacleAvoidance::obstaclecircuit(){
    

    int rawValueFL = lineSensorFrontLeft.getColorCode();
    int rawValueFR = lineSensorFrontRight.getColorCode();
    int rawValueBL = lineSensorBackLeft.getColorCode();
    int rawValueBR = lineSensorBackRight.getColorCode();

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
            steering.setVal(0,0x01FF);
            sendStringln("Auto fährt geradeaus");
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
            steering.setVal(0,0x0000);
            sendStringln("Auto fährt zurück");
            return;
        }

    // Sensor 0 --> dann rechts 
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0   ||
        rawValueFL == 0
        ) {
            steering.setVal(1,240);
            sendStringln("Auto fährt rechts");
            return;
        }

    // Sensor 0 & 1 --> dann rechts 
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0
        ) {
            steering.setVal(1,240);
            sendStringln("Auto fährt rechts");
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
            sendStringln("Auto fährt rechts");
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
            sendStringln("Auto fährt rechts");
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
            sendStringln("Auto fährt rechts");
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
            steering.setVal(1,240);
            sendStringln("Auto fährt rechts");
            return;
        }







    // Sensor 3 --> dann links 
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 0   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0   ||
        rawValueFR == 0
        ) {
            steering.setVal(1,25);
            sendStringln("Auto fährt links");
            return;
        }

    // Sensor 3 & 2 --> dann links 
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 0   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 0   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 0
        ) {
            steering.setVal(1,25);
            sendStringln("Auto fährt links");
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
            sendStringln("Auto fährt links");
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
            sendStringln("Auto fährt links");
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
            sendStringln("Auto fährt links");
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
            sendStringln("Auto fährt links");
            return;
        }






/*

    //int cnt1;
    //int directionleft;

    // vorne frei --> gerade aus fahren
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,128);
            steering.setVal(0,0x01FF);
            sendStringln("Auto fährt geradeaus");
            return;
        }
    // Sensor 0  --> nach rechts drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,255);
            sendStringln("Sensor 0 --> rechts drehen");
            return;
        }

    // Sensor 0 & 1 --> nach rechts drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,255);
            sendStringln("Sensor 0 & 1 --> rechts drehen");
            return;
        }

    // Sensor 0 & 2 --> nach rechts drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,255);
            sendStringln("Sensor 0 & 2 --> rechts drehen");
            return;
        }
    
    // Sensor 0 & 3 --> nach rechst drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,255);
            sendStringln("Sensor 0 & 3 --> rechts drehen");
            return;
        }
    
    // Sensor 0 & 4 --> nach links drehen 
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,0);
            sendStringln("Sensor 0 & 4 --> links drehen");
            return;
        }
    
    // Sensor 0 & 5 --> nach links drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,0);
            sendStringln("Sensor 0 & 5 --> links drehen");
            return;
        }


    // Sensor 0 & 6 --> nach rechts drehen 
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter < 300
        ) {
            steering.setVal(1,255);
            sendStringln("Sensor 0 & 6 --> rechts drehen");
            return;
        }

        // Sensor 3  --> nach links drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 1   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,0);
            sendStringln("Sensor 3 --> links drehen");
            return;
        }


    // redundant
    // Sensor 3 & 0 --> nach links drehen
    /* if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,0);
            return;
        }
    
    
    // Sensor 3 & 1 --> nach links drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 1   && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,0);
            sendStringln("Sensor 3 & 1 --> links drehen");
            return;
        }
    
    // Sensor 3 & 2 --> nach links drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 1   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,0);
            sendStringln("Sensor 3 & 2 --> links drehen");
            return;
        }
    
    // Sensor 3 & 4 --> nach links drehen 
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 1   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,0);
            sendStringln("Sensor 3 & 4--> links drehen");
            return;
        }
    
    // Sensor 3 & 5 --> nach links drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 1   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,0);
            sendStringln("Sensor 3 & 5 --> links drehen");
            return;
        }


    // Sensor 3 & 6 --> nach rechts drehen 
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 1   && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter < 300
        ) {
            steering.setVal(1,255);
            sendStringln("Sensor 3 & 6 --> rechts drehen");
            return;
        }






    // Sensor 1  --> nach rechts drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 1   && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,255);
            return;
        }

    // redundant
    // Sensor 0 & 1 --> nach rechts drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,255);
            return;
        }


    // Sensor 1 & 2 --> nach hinten fahren
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 1   && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(0,0x0000);
            return;
        }
    
    // redundant
    // Sensor 1 & 3 --> nach rechst drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,255);
            return;
        }
    

    // Sensor 1 & 4 --> nach rechts drehen 
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 1   && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,255);
            return;
        }
    
    // Sensor 1 & 5 --> nach rechts drehen
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 1   && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {
            steering.setVal(1,255);
            return;
        }


    // Sensor 1 & 6 --> nach rechts drehen 
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 1   && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter < 300
        ) {
            steering.setVal(1,255);
            return;
        }



    // vorne alles blockiert --> nach hinten fahren 
    /*if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 500 &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {

            steering.setVal(1,128);
            steering.setVal(0,0x0000); 
            return;
        }

    //  rechts blockiert --> nach links fahren
    if (lidarSensors.measureLidar[0].RangeMilliMeter > 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 300 &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[3].RangeMilliMeter < 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {

            steering.setVal(1,0);
            return;
        }

        //  links blockiert --> nach rechts fahren
    if (lidarSensors.measureLidar[0].RangeMilliMeter < 300 && 
        lidarSensors.measureLidar[1].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[2].RangeMilliMeter > 300 &&
        lidarSensors.measureLidar[3].RangeMilliMeter > 300 &&
        lidarSensors.measureLidar[4].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[5].RangeMilliMeter > 1   &&
        lidarSensors.measureLidar[6].RangeMilliMeter > 1
        ) {

            steering.setVal(1,255);
            return;
        }
        

    /*
    // vorne hindernis --> nach rechts drehen
    /* if(lidarSensors.measureLidar[0].RangeMilliMeter < 500 && lidarSensors.measureLidar[3].RangeMilliMeter < 500){
        
        //steering.setVal(0,0x0100);
        steering.setVal(1,255);
        Serial.println("Auto dreht");
        return;

    }
    // vorne links + mitte hindernis --> nach rechts drehen
    if(lidarSensors.measureLidar[1].RangeMilliMeter < 500){
        
        steering.setVal(1,255); //nach rechts drehen
        Serial.println("Auto dreht");
        return;
    }

    // vorne rechst --> nach links drehen
    if(lidarSensors.measureLidar[2].RangeMilliMeter < 500){
        
        steering.setVal(1,0); // nach links drehen
        Serial.println("Auto dreht");
        return;
    }

    // vorne alles blockiert --> nach hinten 
    if(lidarSensors.measureLidar[0].RangeMilliMeter < 100 && lidarSensors.measureLidar[3].RangeMilliMeter < 100 && lidarSensors.measureLidar[2].RangeMilliMeter < 100 && lidarSensors.measureLidar[1].RangeMilliMeter < 100){
        
        steering.setVal(1,128);
        steering.setVal(0,0x0000); // nach hinten fahren
        return;
    }

    // hinten blockiert --> Stopp
    if(lidarSensors.measureLidar[5].RangeMilliMeter < 400){
        
        steering.setVal(0,0x0100); // Stopp
        return;
    }
    /*




    
    /*else if(millis() - cnt1 > 100 && directionleft == 1) {
        directionleft = 2;
        cnt1 = millis();
        steering.setVal(1,0);
        Serial.println("zeitabfrage");
        return;
    }
    
    if(lidarSensors.measureLidar[2].RangeMilliMeter > 250){

        //steering.setVal(0,0x0100);
        steering.setVal(0,0x01FF);
        Serial.println("Auto geradeeee");
        return;
    }
     */

}