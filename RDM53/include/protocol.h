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




void protocolEnter(unsigned char*, size_t);
void autonomous(int);
void remoteControl(unsigned char*);
void calibration(unsigned char*);
void testing(unsigned char*);
void protocolSend(unsigned char, unsigned char, unsigned char, int);
void getValues(uint8_t, uint8_t);