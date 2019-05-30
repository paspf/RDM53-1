/*
 * Main file for RDM53
 * Board: WeMos Lolin32
 * Toochain: Arduino
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 * Project Members: Pascal Pfeiffer, Jan Kühnemund, Taha Tekdemir, Hakim Nasser
 */

#include "main.h"


String inputString = "";
uint8_t inputBinary[128]; 

WebSocketsServer webSocket = WebSocketsServer(81);                // open webSocket Server on port 81


void setup() {
  Serial.begin(115200);
  ESP32Init ESP32InitObj;
  ESP32InitObj.RDMWiFiInit();
  
  webSocket.begin();                                                      // start the websocket server
  webSocket.onEvent(webSocketEvent);                                      // what to do on event...

}

void loop() {
  serialReceive();
  webSocket.loop();
  // Serial.println(millis());
  delay(10);
}
