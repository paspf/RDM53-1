/*
 * Main file for RDM53
 * Board: WeMos Lolin32
 * Toochain: Arduino
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 * Project Members: Pascal Pfeiffer, Jan Kühnemund, Taha Tekdemir (derbabo), Hakim Nasser, Justin Neumann
 */

#include "main.h"


void setup() {
  Serial.begin(230400);
  //ESP32Init ESP32InitObj;
  //ESP32InitObj.RDMWiFiInit();
  interruptInitialization();
  RDMWiFiInit();
  OTAirInit();
  // lidarSensors.initLox();
  webSocket.begin();                                                      // start the websocket server
  webSocket.onEvent(webSocketEvent);                                      // what to do on event...

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
  ArduinoOTA.handle();
  serialReceive();
  webSocket.loop();
  interruptWorkers();
  // Serial.println(millis());
  switch(dC.mode) {
    case 0x0200:
      // Mode : Pause
      break;
    case 0x0100:
      // Remote Control (static)
      webSocket.broadcastTXT("Remote Control static set!");
      // steering.staticEngines();
      //dC.mode = 0x0200;
      break;
    case 0x0101:
      // Remote Control (dynamic)
      webSocket.broadcastTXT("Remote Control dynamic Set!");
      dC.mode = 0x0200;
      break;
    default:
      webSocket.broadcastTXT("Error in dc.Mode");
  }

  // do not use other delays (this should be the only delay in project) !!!!!
  delay(10);

}
