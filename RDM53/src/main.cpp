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
  lidarSensors.initLox();
  webSocket.begin();                                                      // start the websocket server
  webSocket.onEvent(webSocketEvent);                                      // what to do on event...

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  Serial.println("-----------------------");
  Serial.println("RDM53 is ready to go!");
  Serial.println("-----------------------");
}

void loop() {
  //int startTime = millis();
  if(WiFi.status() != WL_CONNECTED && dC.wiFiNotificationSent == false) {
    Serial.println("WiFi lost");
    dC.wiFiNotificationSent = true;
  }
  ArduinoOTA.handle();
  serialReceive();
  webSocket.loop();
  interruptWorkers();
  readSensors();
  // Serial.println(millis());
  switch(dC.mode) {
    case 0x020000:
      // Mode : Pause
      // 11020200000000000012
      break;
    case 0x010000:
      // Remote Control (static)
      // 11020100000000000012
      steering.setPilot();
      break;
    case 0x010001:
      // Remote Control (dynamic)
      webSocket.broadcastTXT("Remote Control dynamic Set!");
      dC.mode = 0x020000;
      break;
    case 0x000000:
      // Autonomous 0
      // 11020000000000000012
      //webSocket.broadcastTXT("Autonomous 0 is set - Line Follower");
      // steering.setVal(0, 0x01FF);
      followLine.followLine();
      steering.setPilot();
      // dC.mode = 0x020000;
      break;
    case 0x000001:
      // Autonomous 1
      webSocket.broadcastTXT("Autonomous 1 is set");
      oi.initobstacledata();
      dC.mode = 0x020000;
      break;
    case 0x000002:
      // Autonomous 2
      webSocket.broadcastTXT("Autonomous 2 is set");
      dC.mode = 0x020000;
      break;
    default:
      webSocket.broadcastTXT("Error in dc.Mode");
  }
  //Serial.print("Runtime: ");
  //Serial.println( millis()- startTime);
  // do not use other delays (this should be the only delay in project) !!!!!
  delay(10);
}
