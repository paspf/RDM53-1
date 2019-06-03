/*
 *This file contains all the protocol functions for the ESP32
 *
 *
 *Author: Jan Kühnemund
 */

#include <stdio.h>
#include <stdlib.h>

unsigned char* protocolEnter(unsigned char* incoming, unsigned char calibrationID)
{
    if(incoming[2]== 3) //receiveData
    {
        if(calibrationID == incoming[4]){
            unsigned char datavalue[4];
            datavalue[1] = incoming[6];
            datavalue[2] = incoming[7];
            datavalue[3] = incoming[8];
            datavalue[4] = incoming[9];
            return datavalue;
        }
    }
    return NULL;
}

char* protocolExit(int dataTypeID, int dataSourceID, long int payload) //sendData
{
    char *exiting = (char*)malloc(sizeof(char) * 15);
    int n;
    n = sprintf(exiting, "01%x%x%x", dataTypeID, dataSourceID, payload);
    return exiting;
}