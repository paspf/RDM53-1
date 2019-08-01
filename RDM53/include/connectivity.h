/*
 * This file contains all connectivity functions
 * for the ESP32
 * 
 * Date: 2019 05 26
 * Author: Pascal Pfeiffer
 */

#ifndef CONNECTIVIY_H

    #define CONNECTIVITY_H
    #include "WebSocketsServer.h"

    // send messages over websocket
    #define SEND_WEBSOCKET 
    // send messages over serial
    //#define SEND_SERIAL

    // unused:
    //#define ERROR_MESSAGES // sould always sent
    //#define DEBUG_LEVEL_0 //highest debug level
    //#define DEBUG_LEVEL_1 //lower debug level

    void analyseString();

    void webSocketEvent(uint8_t, WStype_t, uint8_t *, size_t);

    void serialReceive();

    void sendBinCharArr(unsigned char*, size_t);

    void sendStringln(int);

    void sendStringln(String);

    void sendString(String);

    void sendString(unsigned char *, size_t);

    void sendBinArr(unsigned char *, size_t);

    void sendBinCharArr(unsigned char *, size_t);

#endif