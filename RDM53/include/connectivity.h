/*
 * This file contains all connectivity functions
 * for the ESP32
 * 
 * Date: 2019 05 26
 * Author: Pascal Pfeiffer
 */
#include <Arduino.h>
#include "WebSocketsServer.h"
#include "protocol.h"

#ifndef RDM_MAIN
    extern String inputString;
    extern WebSocketsServer webSocket;
    extern unsigned char inputBinary[128]; 
    extern void protocolEnter(unsigned char*, size_t);
#endif

void analyseString();

void analyseBinary(unsigned char *);


void webSocketEvent(uint8_t, WStype_t, uint8_t *, size_t);

void serialReceive();
