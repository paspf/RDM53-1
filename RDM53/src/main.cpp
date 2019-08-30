/*
 * Main file for RDM53
 * Board: WeMos Lolin32
 * Toochain: Arduino
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 * Project Members: Pascal Pfeiffer, Jan Kühnemund, Taha Tekdemir (derbabo), Justin Neumann
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
  lidarSensors.expanderWrite(EXP_ADDRESS, 0x00);
  Wire.begin();
  Wire1.begin(17,16);
  RDMWiFiInit();
  OTAirInit();
  colTrack.initColTrack();
  lidarSensors.initLox();
  webSocket.begin();                                                      // start the websocket server
  webSocket.onEvent(webSocketEvent);                                      // what to do on event...
  Serial.print("CPU Frequency [Mhz]: ");
  Serial.println(getCpuFrequencyMhz()); //Get CPU clock
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  interruptInitialization();
  mylocation.startMP();
  Serial.print("Creating Task lidarLoop...");
  xTaskCreatePinnedToCore(
                    lidarloop,        // task function.
                    "lidarLoop",      // name of task.
                    10000,            // stack size in word
                    NULL,             // task input parameter
                    2,                // priority of the task
                    &TaskLidarLoop,   // task handle to keep track of created task
                    0);               // pin task to core 0
  
  
  Serial.println("[OK]");                 
  // mylocation.aLittleJoke();
  delay(10);
  piezo.noSound();
  dC.cyclicSensorRefresh = false;
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
  readSensors();

  mylocation.updateLocationVars();

  //ObstTrack.checkForObstacles();

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
      // Autonomous 0 - only line Track
      // 11020000000000000012
      //webSocket.broadcastTXT("Autonomous 0 is set - Line Follower");
      followLine.followLine();
      steering.setPilot();
      break;
    case 0x000001:
      // Autonomous 1 - only lidar
      // 11020000000000000112
      //webSocket.broadcastTXT("Autonomous 1 is set");
      obstaclecircuittest.obstaclecircuit();
      steering.setPilot();
      break;
    case 0x000002:
      // Autonomous 2 - Lidar and LineTracking
      // 11020000000000000212
      // webSocket.broadcastTXT("Autonomous 2 is set");
      driverRDM.driveThroughParcour();
      steering.setPilot();
      break;
    case 0x000003:
      // Autonomous 3 - 
      // 11020000000000000312
      webSocket.broadcastTXT("Autonomous 3 is set");
      dC.mode = 0x020000;
      break;
    case 0x000004:
      // Autonomous 4 - 
      // 11020000000000000412
      webSocket.broadcastTXT("Autonomous 4 is set");
      dC.mode = 0x020000;
      break;
    default:
      webSocket.broadcastTXT("Error in dc.Mode");
      dC.mode = 0x020000;
      dC.cyclicSensorRefresh = false;
  }
  //Serial.print("Runtime: ");
  //Serial.println( millis()- startTime);
  // do not use other delays (this should be the only delay in project) !!!!!
  //delay(10);
}