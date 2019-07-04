/*
 * This file contains all connectivity functions
 * for the ESP32
 * 
 * Date: 2019 05 26
 * Author: Pascal Pfeiffer
 */

#ifndef CONNECTIVIY

    #define CONNECTIVITY
    #include "WebSocketsServer.h"

    #define SEND_WEBSOCKET
    #define SEND_SERIAL
    #define ERROR_MESSAGES // sould always sent
    #define DEBUG_LEVEL_0 //highest debug level
    #define DEBUG_LEVEL_1 //lower debug level

    void analyseString();

    void webSocketEvent(uint8_t, WStype_t, uint8_t *, size_t);

    void serialReceive();

    void sendBinCharArr(unsigned char*, size_t);

    void sendString(int);

    void sendString(unsigned char *, size_t);

    void sendBinArr(unsigned char *, size_t);

    void sendBinCharArr(unsigned char *, size_t);

#endif