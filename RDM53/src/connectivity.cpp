/*
 * This file contains all connectivity functions
 * for the ESP32
 * 
 * Creation date: 2019 05 26
 * Author: Pascal Pfeiffer
 */

// defines
// #define DEBUG_CONNECTIVITY

// includes
#include <Arduino.h>
#include "connectivity.h"
#include "protocol.h"
#include "PublicStructures.h"

// extern objects
extern String inputString;
extern WebSocketsServer webSocket;
extern unsigned char inputBinary[128]; 
extern deviceConfig dC;

// extern functions
extern void protocolEnter(unsigned char*, size_t);

/**
 * Send binary stream to Websocket Client
 * Send String with Hex Numbers over Serial connection
 * @param *charArr Char Array to send
 * @param length length of *charArr
 */
void sendBinCharArr(unsigned char *charArr, size_t length) {
  #ifdef SEND_SERIAL
    Serial.print("Data: ");
    for(int i = 0; i < length; i++) {
      Serial.println(charArr[i], HEX);
    }
  #endif
  #ifdef SEND_WEBSOCKET
    webSocket.broadcastBIN(charArr, length, false);
  #endif
}

/**
 * Send binary Data to Websocket Client
 * Send binary Data over the serial connection as String
 * @param *charArr Char Array to send
 * @param length length of *charArr
 */
void sendBinArr(unsigned char *charArr, size_t length) {
  #ifdef SEND_SERIAL
    for(int i = 0; i < length; i++) {
      Serial.print(charArr[i]);
    }
  #endif
  #ifdef SEND_WEBSOCKET
    webSocket.broadcastBIN(charArr, length, false);
  #endif
}

/**
 * Send a String to Websocket Client 
 * Send a String over the serial connection
 * @param *charArr Char Array to send
 * @param length length of *charArr
 */
void sendString(unsigned char *charArr, size_t length) {
  #ifdef SEND_SERIAL
    Serial.println("Data: ");
    for(int i = 0; i < length; i++) {
      Serial.print(charArr[i], HEX);
    }
  #endif
  #ifdef SEND_WEBSOCKET
    webSocket.broadcastTXT(charArr, length, false);
  #endif
}

/**
 * Send a String to Websocket Client 
 * Send a String over the serial connection
 * @param str String to send
 */
void sendString(String str) {
  #ifdef SEND_SERIAL
    Serial.print(str);
  #endif
  #ifdef SEND_WEBSOCKET
   webSocket.broadcastTXT(str);
  #endif
}

/**
 * Send a String to Websocket Client 
 * Send a String over the serial connection (with newline)
 * @param str String to send
 */
void sendStringln(String str) {
  #ifdef SEND_SERIAL
    Serial.println(str);
  #endif
  #ifdef SEND_WEBSOCKET
   webSocket.broadcastTXT(str);
  #endif
}

/**
 * Send a String to Websocket Client 
 * Send a String over the serial connection (with newline)
 * @param value int value to send
 */
void sendStringln(int value) {
  #ifdef SEND_SERIAL
    Serial.println(value);
  #endif
  #ifdef SEND_WEBSOCKET
    char charArr[64];
    sprintf(charArr, "%i", value);
    webSocket.broadcastTXT(charArr, 64);
  #endif
}


/**
 * Analyse Strings and interpret commands
 */
void analyseString() {
  sendString("String received: ");
  sendStringln(inputString);
  if(inputString.startsWith("DEBUG ")) {
    if(inputString.substring(6, 18) == "protocolSend") {
      Serial.println("protocolSend():");
        protocolSend(0x82, 0x93, 0x30, (int) 0xFE203040);
        // webSocket.broadcastTXT("DEBUG protocolSend is working!!!!!");
    }
    if(inputString.substring(6, 28) == "dC.cyclicSensorRefresh") {
      dC.cyclicSensorRefresh = !dC.cyclicSensorRefresh;
      Serial.print("dC.cyclicSensorRefresh: ");
      if(dC.cyclicSensorRefresh == true) {
        Serial.println("[ON]");
      }
      else {
        Serial.println("[OFF]");
      }
    }
  }
  inputString = "";
}

/**
 * webSocketEvent, based on:
 * WebSockets 2.0.5
 * by Markus Sattler
 * https://github.com/Links2004/arduinoWebSockets 
 * This function is called when data by a
 * websocket client is received
 * @param num number of the websocket client
 * @param type type of the websocket event
 * @param *payload transferred data
 * @param length  length of the payload
 */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n\r", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n\r", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, "Connected to RDM 53");                       // send message to client
      }
      break;
    case WStype_TEXT:                                          // Receive Text
      //Serial.printf("[%u] get Text: %s\n\r", num, payload);// shows client number an received text
      inputString = (char*)payload;                         // transform uint8_t to String
      analyseString();
  
      // webSocket.sendTXT(num, "message here");            // send message to client
      // webSocket.broadcastTXT("message here");            // send data to all connected clients
      break;
    case WStype_BIN:                                          // Receive binary data
        //Serial.printf("[%u] get binary lenght: %u\n", num, length);
        if (length > 128)
            Serial.printf("Binary Data to large");
        else {
          //Serial.println("Web bin!");
          // analyseBinary(payload, lenght);
          protocolEnter(payload, length);
          // hexdump(payload, lenght);
        }
      // webSocket.sendBIN(num, payload, lenght);           // send binary message to client
      break;
    default:
      yield();
  }
}

/**
 * serialReceive, based on:
 * https://www.arduino.cc/en/Tutorial/SerialEvent
 * 
 * This function is called when serial data has been
 * received
 * The bytes are spitted into the format for the RDM53 protocol 
 */
void serialReceive() {
  bool binary = true;
  size_t lastEl = 0;
  while (Serial.available()) {
    char inChar = (char)Serial.read();                          // get the new byte
    // check if a string is received
    if(lastEl == 0 && inChar != 0x11) {
      binary = false;
    }
    // check if binary data is received
    if(lastEl == 0 && inChar == 0x11) {
      #ifdef DEBUG_CONNECTIVITY
      webSocket.broadcastTXT("0x11 found!");
      #endif
      lastEl++;
      continue;
    }
    // if the incoming character is NOT a carriage return...
    // add it to the inputString
    if (inChar != '\r' && binary == false) {
      inputString += inChar;
    }
    // check if the binaryEnd byte has been received
    else if(binary == true && lastEl == 9 && inChar == 0x12) {
      protocolEnter(inputBinary, 128);
    }
    // add received byte to the inputBinary array
    else if(binary == true && lastEl < 10) {
      inputBinary[lastEl] = inChar;
    }
    // no binary received, analyse received string
    else if(binary == false) {
      analyseString();
      #ifdef DEBUG_CONNECTIVITY
      webSocket.broadcastTXT("Analyse String");
      #endif
    }
    // just read the input and throw it away
    else { 
      Serial.read();
    }
    lastEl++;
  }
}
