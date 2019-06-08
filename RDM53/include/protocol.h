/*This file contains header information about the protocol.cpp
 *
 *
 *
 *
 *
 *Author: Jan Kühnemund
 */

#include <stdio.h>

int protocolEnter(unsigned char* incoming, size_t length);
void autonomous(int autonomyNum);
void remoteControl(int payload);
void calibration(char ID, int value);
void testing(char testID, int value);
void protocolSend(char* buffer, int dataType, int dataSource, int mode, int payload);