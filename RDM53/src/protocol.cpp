/*
 *This file contains all the protocol functions for the ESP32
 *
 *
 *Author: Jan Kühnemund
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <protocol.h>


int protocolEnter(unsigned char* incoming, size_t length)
{
    int payload;
    payload = incoming[5]<<3 + incoming[6]<<2 + incoming[7]<<1 + incoming[8];

    if(incoming[1]== 2) //switchMode
    {
        switch (incoming[2])
        {
        case 0x0: //Autonomous
            //Start thread autonomy and breaking all other threads.
            autonomous(payload);
            break;
        case 0x1: //RemoteControl
            //Start thread remotecontrol and break other threads.
        case 0x2:
            //Break Autonomy and RemoteControl. DeepSleepMode?
        case 0x3:
            //
        default:
            break;
        }
    }
    if(incoming[1 == 3]){ //receiveData
        switch (incoming[2])
        {
        case 0x0: //RemoteControlData
            remoteControl(payload);

            break;
        case 0x1: //Calibration
            calibration(incoming[2], payload);
        case 0x2: //Testing
            testing(incoming[2], payload);
        default:
            break;
        }
    }
    return NULL;
}

void autonomous(int autonomyNum)
{ 
    //Put functions leading to Autonomy here
    int n;
    char buffer[64];
    n = sprintf(buffer, "Autonomy mode %x will now commence", autonomyNum);
}

void remoteControl(int payload)
{
    //speed(strtol(payload, NULL, 16) / 0xFF);
    //turnrate(strtol(payload, NULL, 16) % 0xFF);
    int n;
    char buffer[64];
    n = sprintf(buffer, "Calibration ID %x would now have Calibration Value: %x", payload);
}

void calibration(char ID, int value){
    int n;
    char buffer[64];
    n = sprintf(buffer, "Calibration ID %c would now have Calibration Value: %x", ID, value);
}

void testing(char testID, int value){
    int n;
    char buffer[64];
    n = sprintf(buffer, "Testing %c with value %x", testID, value);
}

void protocolSend(char* buffer, int dataType, int dataSource, int mode, int payload){
    int tmp;
    tmp = sprintf(buffer, "01%x%x%x%x", dataType, dataSource, mode, payload);
}