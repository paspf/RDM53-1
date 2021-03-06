/*
 * This file contains all defines and global objects
 * 
 * Creation date: 2019 05 21
 * Author: Pascal Pfeiffer
 */

/**** includes ****/
#define RDM_MAIN

#include "WebSocketsServer.h"
#include "ESP32Init.h"
#include "connectivity.h"
#include "protocol.h"
#include "lineTracking.h"
#include "EnginesInterface.h"
#include "steering.h"
#include "lidar.h"
#include "interruptRoutines.h"
#include "ObstacleInterface.h"
#include "piezo.h"
#include "HCSR04P.h"
#include "LineFollower.h"
#include "readSensors.h"
#include "ObstacleAvoidance.h"
#include "ObstacleAndLine.h"
#include "esp32-hal-cpu.h"
#include "ObstacleTracker.h"
#include "colorTracking.h"
#include "obstacleAndLineRNGLab.h"

/**** structures ****/
#include "PublicStructures.h"

/**** Public Values ****/
String inputString = "";
unsigned char inputBinary[128]; 


/**** Public structures ****/
deviceConfig dC;
obstBuff obstBuffer;

/**** Public Objects ****/
WebSocketsServer webSocket = WebSocketsServer(81);                // open webSocket Server on port 81
lineTrackInterface lineSensorFrontLeft(LT_FL_PIN);
lineTrackInterface lineSensorFrontRight(LT_FR_PIN);
lineTrackInterface lineSensorBackLeft(LT_BL_PIN);
lineTrackInterface lineSensorBackRight(LT_BR_PIN);

EnginesInterface enginesInt;
SteeringInterface steering;

lidar lidarSensors;

Location mylocation;

PiezoInterface piezo;

HCSR04P ultraSonic(2, 15, 4000);

LineFollower followLine;
ObstacleAvoidance obstaclecircuittest;
ObstacleAndLine driverRDM;
ObstacleAndLine_RNGLAB driverRNG;

ObstacleTracker ObstTrack;

ColTrack colTrack;