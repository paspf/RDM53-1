/*
 * This file contains all defines and global values
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 */

// #include <Arduino.h>
#define RDM_MAIN

#include "WebSocketsServer.h"
#include "mainUSR.h"
#include "ESP32Init.h"
#include "connectivity.h"
#include "protocol.h"


/* extern "C" {
   #include "connectivity.cpp"
} */


/* Public Values */
String inputString = "";
unsigned char inputBinary[128]; 
WebSocketsServer webSocket = WebSocketsServer(81);                // open webSocket Server on port 81