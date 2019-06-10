/*This file contains header information about the protocol.cpp
 *
 *
 *
 *
 *
 *Author: Jan Kühnemund
 */

#include <stdio.h>
#include <Arduino.h>
#include <WebSocketsServer.h>
// #include "PublicStructures.h"


#ifndef RDM_MAIN
    extern String inputString;
    extern WebSocketsServer webSocket;
    #include "PublicStructures.h"
    extern deviceConfig dC;
#endif

int protocolEnter(unsigned char*, size_t);
void autonomous(int);
void remoteControl(unsigned char*);
void calibration(unsigned char*);
void testing(unsigned char*);
void protocolSend(unsigned char, unsigned char, unsigned char, int);