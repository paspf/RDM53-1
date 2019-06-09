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

int protocolEnter(unsigned char*, size_t);
void autonomous(int);
void remoteControl(unsigned char*);
void calibration(char, int);
void testing(char, int);
void protocolSend(unsigned char, unsigned char, unsigned char, int);