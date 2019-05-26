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
  inputString = "";
}

/*
 * Analyse bytewise transmitted data
 */
void analyseBinary(unsigned char *inputBinary) {
  Serial.println("Binary received");
}

/*
  webSocketEvent, based on:
  WebSockets 2.0.5
  by Markus Sattler
  https://github.com/Links2004/arduinoWebSockets
 */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n\r", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n\r", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, "Connected");                       // send message to client
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
        Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
        if (lenght > 128)
            Serial.printf("Binary Data to large");
        else
            analyseBinary(payload);
        /* else {
            for(int i = 0; i < lenght; i++) {
                inputBinary[i] = payload[i];
            }
        } */
        // hexdump(payload, lenght);
  
      // webSocket.sendBIN(num, payload, lenght);           // send binary message to client
      break;
  }
}
/*
 serialReceive, based on:
 https://www.arduino.cc/en/Tutorial/SerialEvent
 */
void serialReceive() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();                          // get the new byte
    if (inChar != '\r') {                                       // if the incoming character is NOT a carriage return...
      inputString += inChar;                                    // add it to the inputString
    }
    else {                                                      // if the incoming character is a carriage return, set a flag
      analyseString();
    }
  }
}
