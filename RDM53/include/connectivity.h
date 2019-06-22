/*
 * This file contains all connectivity functions
 * for the ESP32
 * 
 * Date: 2019 05 26
 * Author: Pascal Pfeiffer
 */
#include "WebSocketsServer.h"

#define SEND_WEBSOCKET
#define SEND_SERIAL

void analyseString();

void analyseBinary(unsigned char *);

void webSocketEvent(uint8_t, WStype_t, uint8_t *, size_t);

void serialReceive();

void sendBinCharArr(unsigned char*, size_t);