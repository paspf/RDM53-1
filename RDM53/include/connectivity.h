/*
 * This file contains all connectivity functions
 * for the ESP32
 * 
 * Date: 2019 05 26
 * Author: Pascal Pfeiffer
 */
#include <Arduino.h>
#include "WebSocketsServer.h"


void analyseString();

void analyseBinary(unsigned char *);


void webSocketEvent(uint8_t, WStype_t, uint8_t *, size_t);

void serialReceive();
