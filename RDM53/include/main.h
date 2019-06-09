/*
 * This file contains all defines and global values
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 */

/**** includes ****/
#define RDM_MAIN

#include "WebSocketsServer.h"
#include "mainUSR.h"
#include "ESP32Init.h"
#include "connectivity.h"
#include "protocol.h"
#include "lineTracking.h"


/**** structures ****/
/*
 * This struct is used for the device states and modes
 * mode:
 * 0: Do nothing
 * 1 : ...
 * 2 : ...
 */
struct deviceConfig {
    short mode = 0;
};


/**** Public Values ****/
String inputString = "";
unsigned char inputBinary[128]; 


/**** Public structures ****/
deviceConfig dC;

/**** Public Objects ****/
WebSocketsServer webSocket = WebSocketsServer(81);                // open webSocket Server on port 81
lineTrackInterface lineSensorFrontLeft(36);
lineTrackInterface lineSensorFrontRight(39);
lineTrackInterface lineSensorBackLeft(34);
lineTrackInterface lineSensorBackRight(32);
