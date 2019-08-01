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

TaskHandle_t TaskLidarLoop;

/*
 * Arudino Setup
 * FreeRTOS
 * Execution Priority : 1
 * Execution Core : 1
 */
void setup() {
  Serial.begin(230400);
  //ESP32Init ESP32InitObj;
  //ESP32InitObj.RDMWiFiInit();
  RDMWiFiInit();
  OTAirInit();
  lidarSensors.initLox();
  webSocket.begin();                                                      // start the websocket server
  webSocket.onEvent(webSocketEvent);                                      // what to do on event...
  Serial.print("CPU Frequency [Mhz]: ");
  Serial.println(getCpuFrequencyMhz()); //Get CPU clock
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  interruptInitialization();
  Serial.print("Creating Task lidarLoop...");
  xTaskCreatePinnedToCore(
                    lidarloop,        /* task function. */
                    "lidarLoop",      /* name of task. */
                    10000,            /* stack size in words*/
                    NULL,             /* task input parameter */
                    2,                /* priority of the task */
                    &TaskLidarLoop,   /* task handle to keep track of created task */
                    0);               /* pin task to core 0 */ 
  Serial.println("[OK]");                 
  Serial.println("-----------------------");
  Serial.println("RDM53 is ready to go!");
  Serial.println("-----------------------");
}

/*
 * Arudino Main loop
 * FreeRTOS
 * Execution Priority : 1
 * Execution Core : 1
 */
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

  if(dC.cyclicSensorRefresh == true) {
    readSensors();
  }
  
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
      followLine.followLine();
      steering.setPilot();
      // dC.mode = 0x020000;
      break;
    case 0x000001:
      // Autonomous 1
      //webSocket.broadcastTXT("Autonomous 1 is set");
      obstaclecircuittest.obstaclecircuit();
      steering.setPilot();
      //dC.mode = 0x020000;
      //webSocket.broadcastTXT("Autonomous 1 is set");
      // dC.mode = 0x020000;
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
  //delay(10);
}