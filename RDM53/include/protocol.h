/*This file contains header information about the protocol.cpp
 *
 *
 *
 *
 *
 *Author: Jan Kühnemund
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdio.h>
#include <Arduino.h>
#include <WebSocketsServer.h>
#include "PublicStructures.h"
#include "location.h"


extern Location mylocation;

void protocolEnter(unsigned char*, size_t);
void autonomous(unsigned char);
void remoteControl(unsigned char*, int);
void calibration(unsigned char*, int);
void testing(unsigned char*, int);
void protocolSend(unsigned char, unsigned char, unsigned char, int);
void getValues(uint8_t, uint8_t);

#endif