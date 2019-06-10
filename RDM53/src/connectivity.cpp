/*
 * This file contains all connectivity functions
 * for the ESP32
 * 
 * Date: 2019 05 26
 * Author: Pascal Pfeiffer
 */
#include "connectivity.h"

/*
 * Analyse Strings and interpret commands
 */
void analyseString() {
  Serial.print("String received:");
  Serial.println(inputString);
  webSocket.broadcastTXT(inputString);
  if(inputString.startsWith("DEBUG ")) {
    if(inputString.substring(6, 18) == "protocolSend") {
        // protocolSend(unsigned char dataType, unsigned char dataSource, unsigned char mode, int payload)
        webSocket.broadcastTXT("DEBUG protocolSend is working!!!!!");
    }
    // call jans function
  }
  inputString = "";
}

/*
 * Analyse bytewise transmitted data
 */
void analyseBinary(unsigned char *inputBinary, size_t length) {
  Serial.println("Binary received:");
  Serial.printf((char*)inputBinary);
  Serial.println();
  if(length > 1) {
      if(inputBinary[0] == 0x1) {
    Serial.println("Byte 1 = 0x1");
    if(inputBinary[1] == 0x8F)
      Serial.println("Byte 2 = 0x8F");
    }
  }
}

/*
  webSocketEvent, based on:
  WebSockets 2.0.5
  by Markus Sattler
  https://github.com/Links2004/arduinoWebSockets
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
        Serial.printf("[%u] get binary lenght: %u\n", num, length);
        if (length > 128)
            Serial.printf("Binary Data to large");
        else {
          Serial.println("Web bin!");
          // analyseBinary(payload, lenght);
          protocolEnter(payload, length);
          // hexdump(payload, lenght);
        }
      // webSocket.sendBIN(num, payload, lenght);           // send binary message to client
      break;
  }
}
/*
 serialReceive, based on:
 https://www.arduino.cc/en/Tutorial/SerialEvent
 */
void serialReceive() {
  bool binary = true;
  size_t lastEl = 0;
  while (Serial.available()) {
    char inChar = (char)Serial.read();                          // get the new byte
    if(lastEl == 0 && inChar != 0x11) {
      binary = false;
    }
    if(lastEl == 0 && inChar == 0x11) {
      webSocket.broadcastTXT("0x11 found!");
      lastEl++;
      continue;
    }
    if (inChar != '\r' && binary == false) {                                       // if the incoming character is NOT a carriage return...
      inputString += inChar;                                    // add it to the inputString
    }
    else if(binary == true && lastEl == 9 && inChar == 0x12) {
      protocolEnter(inputBinary, 128);
      webSocket.broadcastTXT("line 104");
    }
    else if(binary == true && lastEl < 10) {
      inputBinary[lastEl] = inChar;
      webSocket.broadcastTXT("line 100");
    }
    else if(binary == false) {
      analyseString();
      webSocket.broadcastTXT("Analyse String");
    }
    else {                                                      // if the incoming character is a carriage return, set a flag
      Serial.read();
      // webSocket.broadcastTXT("line 112 length:");
      // char lastElStr[100];
      // sprintf(lastElStr, "%d", lastEl);
      // webSocket.broadcastTXT(lastElStr);
    }
    lastEl++;
  }
}
