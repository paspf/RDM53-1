/*
 *This file contains all the protocol functions for the ESP32
 *
 *
 *Author: Jan Kühnemund, Pascal Pfeiffer
 */

#include <protocol.h>
#include "connectivity.h"
#include "HCSR04P.h"
#include "colorTracking.h"
#include "ObstacleAndLine.h"

#ifndef RDM_MAIN
    extern String inputString;
    extern WebSocketsServer webSocket;
    #include "PublicStructures.h"
    extern deviceConfig dC;
    #include <EnginesInterface.h>
    extern EnginesInterface enginesInt;
    #include "steering.h"
    extern SteeringInterface steering;
    #include "ESP32Init.h"
    extern HCSR04P ultraSonic;
    #include "lidar.h"
    extern lidar lidarSensors;
    #include "lineTracking.h"
    extern lineTrackInterface lineSensorFrontLeft;
    extern lineTrackInterface lineSensorFrontRight;
    extern lineTrackInterface lineSensorBackLeft;
    extern lineTrackInterface lineSensorBackRight;
    extern ColTrack colTrack;
    extern ObstacleAndLine driverRDM;
#endif



/**
 * protocolEnter() takes the incoming protocol and sends the commands to
 * their corresponding functions.
 */
void protocolEnter(unsigned char* incoming, size_t length)
{
    int payload = 0;
    /*Serial.println("Payload JAN:");
    Serial.println(incoming[5], HEX);
    Serial.println(incoming[6], HEX);
    Serial.println(incoming[7], HEX);
    Serial.println(incoming[8], HEX);
    Serial.println("END PAYLOAD JAN");*/
    //webSocket.broadcastTXT("test");
    //Serial.println("test");
    

    payload = (incoming[5]<<24) | (incoming[6]<<16) | (incoming[7]<<8) | incoming[8];
    // Serial.println(payload, HEX);

    if(incoming[1]== 2) //switchMode
    {
        // Select device mode
        switch (incoming[2])
        {
        case 0x0: //Autonomous
            //Start thread autonomy and breaking all other threads.
            // payload == autonomous mode
            autonomous(incoming[8]);
            break;
        case 0x1: //RemoteControl
            //Start thread remotecontrol and break other threads.
            // 11 02 01 00 00 00 00 00 00-0f 12 (static)
            // 11 02 01 00 00 00 00 00 10-1f 12 (dynamic)
            sendString("Remote Control Mode");
            dC.mode = 0x010000 | incoming[8];
            dC.cyclicSensorRefresh = false;
            //Serial.println(dC.mode, HEX);
            break;
        case 0x2:
            //Break Autonomy and RemoteControl. DeepSleepMode?
            // 11 02 02 00 00 00 00 00 00 00 12
            sendString("Pause Mode");
            dC.mode = 0x20000;
            dC.cyclicSensorRefresh = false;
            enginesInt.stopE();
            break;
        case 0x3:
            sendString("Restart ESP in 1 second...");
            delay(20);
            yield();
            delay(980);
            esp_restart();
            break;
        default:
            sendString("ERROR: protocolEnter: Undefined Mode set due to invalid incoming[2]");
            break;
        }
    }
    if(incoming[1] == 3){ //receiveData
        switch (incoming[2])
        {
        case 0x0: //RemoteControlData
            remoteControl(incoming, payload);
            //webSocket.broadcastTXT("remotControl");
            //Serial.println("remoteControl");
            break;
        case 0x1: //Calibration
            calibration(incoming, payload);
            break;
        case 0x2: //Testing
            testing(incoming, payload);
            break;
        case 0x3: //GetValues
            getValues(incoming[3], incoming[4]);
            break;
        default:
            webSocket.broadcastTXT("Error: protocolEnter: Undefined ReceiveData");
            break;
        }
    }
}
/**
 * autonomous is responsible for calling functions that will be part of the autonomous mode.
 * 
 */
void autonomous(unsigned char autonomyNum)
{ 
    //Put functions leading to Autonomy here
    sendString("Autonomous mode");
    dC.mode = 0x0000 + autonomyNum;
    dC.cyclicSensorRefresh = true;
    switch(dC.mode) {
    case 0x000001:
    case 0x000002:
      // Autonomous 2 - Lidar and LineTracking
      // 11020000000000000212
      driverRDM.initValues();
      break;
    }
}
/**
 * remoteControl() is responsible for giving the steering function its payload.
 * Please implement steering function.
 */
void remoteControl(unsigned char* incoming, int payload)
{
    switch (incoming[3])
        {
        case 0x0: // forward / backward
            // 11 03 00 00 00 00 00 0x xx 12
            steering.setVal(0, payload);
            break;
        case 0x1: // turn value
            // 11 03 00 01 00 00 00 00 xx 12
            steering.setVal(1, payload);
            break;
        default:
            Serial.println("Error: remoteControl");
            webSocket.broadcastTXT("Error: remoteControl");
            break;
        }
}

void calibration(unsigned char * incoming, int payload){
    // if something needs calibration this function will be filled
}
/**
 * testing() takes a 4 Byte unsigned Char and tests the variables that are to be tested.
 *Please implement return values and testing function.
 * 
 */
void testing(unsigned char* incoming, int payload){
    switch (incoming[3]){
        case 0x0:
            // 11 03 02 00 00 00 00 00 B0 12
            enginesInt.setEFL(incoming[7],incoming[8]);
            break;
        case 0x1:
            // 11 03 02 01 00 00 00 00 50 12
            enginesInt.setEFR(incoming[7],incoming[8]);
            break;
        case 0x2:
            // 11 03 02 02 00 00 00 00 FF 12
            enginesInt.setEBL(incoming[7],incoming[8]);
            break;
        case 0x3:
            // 11 03 02 03 00 00 00 00 FF 12
            enginesInt.setEBR(incoming[7],incoming[8]);
            break;
        case 0x4:
            // 11 03 02 04 00 00 00 00 00 12
            enginesInt.stopE();
            break;
        case 0x5:
            //piezo(payload);
            break;
        case 0x6:
            // 11 03 02 06 00 00 00 00 00 12
            dC.cyclicSensorRefresh = !dC.cyclicSensorRefresh;
            if(dC.cyclicSensorRefresh == true) {
                sendStringln("dC.cyclicSensorRefresh Enabled!");
            }
            else {
                sendStringln("dC.cyclicSensorRefresh Disabled!");
            }
            break;
        default:
            Serial.println("Error in testing() due to invalid incoming[3]");
            break;
    }
}

/**
 * This function takes the incoming protocol, looks up which sensor value 
 * is looked for and calls that value. It then tells protocolSend to send that value.
 *  
 */
void getValues(uint8_t dataSource, uint8_t dataSubSource){
    int32_t dummy = 0;
    /*
     * xx xx xx DS SS ---PAYLOAD---
     * DS: dataSource
     * SS: dataSubSource
     */ 
    switch (dataSource)
    {
    case 0x0: // Lidar 0
        switch(dataSubSource) 
        {
            case 0x0: // lidar 0
                // 11 03 03 00 00 00 00 00 00 12
                lidarSensors.readLOXSensors();
                protocolSend(0x0, dataSource, dataSubSource, lidarSensors.measureLidar[0].RangeMilliMeter);
                break;
            case 0xFF:
                // 11 03 03 00 FF 00 00 00 00 12
                lidarSensors.readLOXSensors();
                lidarSensors.printLOXValues();
            break;
        }
        break;
    case 0x1: // Lidar 1
        // 11 03 03 01 00 00 00 00 00 12
        lidarSensors.readLOXSensors();
        protocolSend(0x0, dataSource, dataSubSource, lidarSensors.measureLidar[1].RangeMilliMeter);
        break;
    case 0x2: // Lidar 2
        // 11 03 03 02 00 00 00 00 00 12
        lidarSensors.readLOXSensors();
        protocolSend(0x0, dataSource, dataSubSource, lidarSensors.measureLidar[2].RangeMilliMeter);
        break;
    case 0x3: // Lidar 3
        // 11 03 03 03 00 00 00 00 00 12
        lidarSensors.readLOXSensors();
        protocolSend(0x0, dataSource, dataSubSource, lidarSensors.measureLidar[3].RangeMilliMeter);
        break;
    case 0x4: // Lidar 4
        // 11 03 03 04 00 00 00 00 00 12
        lidarSensors.readLOXSensors();
        protocolSend(0x0, dataSource, dataSubSource, lidarSensors.measureLidar[4].RangeMilliMeter);
        break;
    case 0x5: // Lidar 5
        // 11 03 03 05 00 00 00 00 00 12
        lidarSensors.readLOXSensors();
        protocolSend(0x0, dataSource, dataSubSource, lidarSensors.measureLidar[5].RangeMilliMeter);
        break;
    case 0x6: // Lidar 6
        // 11 03 03 06 00 00 00 00 00 12
        lidarSensors.readLOXSensors();
        protocolSend(0x0, dataSource, dataSubSource, lidarSensors.measureLidar[6].RangeMilliMeter);
        break;
    case 0x7: // Ultraschall 1
        // 11 03 03 07 00 00 00 00 00 12
        protocolSend(0x0, dataSource, dataSubSource, dummy);
        break;
    case 0x8: // Ultraschall 2
        // 11 03 03 08 00 00 00 00 00 12
        protocolSend(0x0, dataSource, dataSubSource, ultraSonic.measureDist());
        break;
    case 0x9: //Gyroscope
        switch (dataSubSource)
        {
        case 0x0:   //X-Achse
            //webSocket.broadcastBIN(dataSubSource, sizeof(dataSubSource));
            protocolSend(0x0, dataSource, dataSubSource, mylocation.getGyrX());
            break;
        case 0x1:   //Y-Achse
            //webSocket.broadcastBIN(dataSubSource, sizeof(dataSubSource));
            protocolSend(0x0, dataSource, dataSubSource, mylocation.getGyrY());
            break;
        case 0x2:   //Z-Achse
            //webSocket.broadcastBIN(dataSubSource, sizeof(dataSubSource));
            protocolSend(0x0, dataSource, dataSubSource, mylocation.getGyrZ());
            break;

        default:
            webSocket.broadcastTXT("Error: Gyroscope Unknown dataSubSource query");
            break;
        }
        break;
    case 0xA: //Accelerometer
        switch (dataSubSource)
        {
        case 0x0:   //X-Achse
            //webSocket.broadcastBIN(dataSubSource, sizeof(dataSubSource));
            protocolSend(0x0, dataSource, dataSubSource, mylocation.getAccX());
            break;
        case 0x1:   //Y-Achse
            //webSocket.broadcastBIN(dataSubSource, sizeof(dataSubSource));
            protocolSend(0x0, dataSource, dataSubSource, mylocation.getAccY());
            break;
        case 0x2:   //Z-Achse
            //webSocket.broadcastBIN(dataSubSource, sizeof(dataSubSource));
            protocolSend(0x0, dataSource, dataSubSource, mylocation.getAccZ());
            break;

        default:
            webSocket.broadcastTXT("Error: Accelerometer Unknown dataSubSource query");
            break;
        }
        break;
    case 0xB: //Magnetometer
        switch (dataSubSource)
        {
        case 0x0:   //X-Achse
            //webSocket.broadcastBIN(dataSubSource, sizeof(dataSubSource));
            protocolSend(0x0, dataSource, dataSubSource, mylocation.getMagX());
            break;
        case 0x1:   //Y-Achse
            //webSocket.broadcastBIN(dataSubSource, sizeof(dataSubSource));
            protocolSend(0x0, dataSource, dataSubSource, mylocation.getMagY());
            break;
        case 0x2:   //Z-Achse
            //webSocket.broadcastBIN(dataSubSource, sizeof(dataSubSource));
            protocolSend(0x0, dataSource, dataSubSource, mylocation.getMagZ());
            break;

        default:
            webSocket.broadcastTXT("Error: Magnetometer Unknown dataSubSource query");
            break;
        }
        break;
    case 0xC: // Phototransistor front left raw value
        // 11 03 03 0C 00 00 00 00 00 12
        lineSensorFrontLeft.readSensor();
        Serial.println(lineSensorFrontLeft.getRawSensorValue());
        protocolSend(0x0, dataSource, dataSubSource, lineSensorFrontLeft.getRawSensorValue());
        break;
    case 0xD: // Phototransistor front right raw value
        // 11 03 03 0D 00 00 00 00 00 12
        lineSensorFrontRight.readSensor();
        Serial.println(lineSensorFrontRight.getRawSensorValue());
        protocolSend(0x0, dataSource, dataSubSource, lineSensorFrontRight.getRawSensorValue());
        break;
    case 0xE: // Phototransistor back left raw value
        // 11 03 03 0E 00 00 00 00 00 12
        lineSensorBackLeft.readSensor();
        Serial.println(lineSensorBackLeft.getRawSensorValue());
        protocolSend(0x0, dataSource, dataSubSource, lineSensorBackLeft.getRawSensorValue());
        break;
    case 0xF: // Phototransistor back right raw value
        // 11 03 03 0F 00 00 00 00 00 12
        lineSensorBackRight.readSensor();
        Serial.println(lineSensorBackRight.getRawSensorValue());
        protocolSend(0x0, dataSource, dataSubSource, lineSensorBackRight.getRawSensorValue());
        break;
    case 0x10: // battery percentage
        // 11 03 03 10 00 00 00 00 00 12
        protocolSend(0x0, dataSource, dataSubSource, getBatteryPercentage());
        break;
    case 0x11: // line color front left
        // 11 03 03 11 00 00 00 00 00 12
        lineSensorFrontLeft.readSensor();
        protocolSend(0x0, dataSource, dataSubSource, lineSensorFrontLeft.getColorCode());
        break;
    case 0x12: // line color front right
        // 11 03 03 12 00 00 00 00 00 12
        lineSensorFrontRight.readSensor();
        protocolSend(0x0, dataSource, dataSubSource, lineSensorFrontRight.getColorCode());
        break;
    case 0x13: // line color back left
        // 11 03 03 13 00 00 00 00 00 12
        lineSensorBackLeft.readSensor();
        protocolSend(0x0, dataSource, dataSubSource, lineSensorBackLeft.getColorCode());
        break;
    case 0x14: // line color back right
        // 11 03 03 14 00 00 00 00 00 12
        lineSensorBackRight.readSensor();
        protocolSend(0x0, dataSource, dataSubSource, lineSensorBackRight.getColorCode());
        break;
    case 0x15: // enable / disable wiFiNotificationSender
        // 11 03 03 15 00 00 00 00 00 12
        dC.wiFiNotificationSender = !dC.wiFiNotificationSender;
        if(dC.wiFiNotificationSender == true) {
            dC.cyclicSensorRefresh = true;
            sendStringln("wiFiNotificationSender: ENABLED");
        }
        else {
            sendStringln("wiFiNotificationSender: DISABLED");
        }
        break;
    case 0x16 : // Heading
        // 11 03 03 16 00 00 00 00 00 12
        protocolSend(0x0, dataSource, dataSubSource, mylocation.getHeading());
        break;
    case 0x17 : // Pitch
        // 11 03 03 17 00 00 00 00 00 12
        protocolSend(0x0, dataSource, dataSubSource, mylocation.getPitch());
        break;
    case 0x18 : // Roll
        // 11 03 03 18 00 00 00 00 00 12
        protocolSend(0x0, dataSource, dataSubSource, mylocation.getRoll());
        break;
    case 0x19 : // Speed (from arduino)
        // 11 03 03 19 00 00 00 00 00 12
        // protocolSend(0x0, dataSource, dataSubSource, mylocation.getSpeedTrue());
        break;
    case 0x1A: // color sensor
        // 11 03 03 1A 00 00 00 00 00 12
        colTrack.readSensor();
        protocolSend(0x0, dataSource, dataSubSource, colTrack.getLTcolor());
        break; 
    case 0x1B: // speed combined
        // 11 03 03 1B 00 00 00 00 00 12
        Serial.print("Speed: ");
        Serial.println(mylocation.speedCombined2);
        protocolSend(0x0, dataSource, dataSubSource, (float) mylocation.speedCombined2);
        break;
    case 0x1C: // speed left
        // 11 03 03 1C 00 00 00 00 00 12
        protocolSend(0x0, dataSource, dataSubSource, (float) mylocation.speedLeft2);
        break;
    case 0x1D: // speed right
        // 11 03 03 1D 00 00 00 00 00 12
        protocolSend(0x0, dataSource, dataSubSource, (float) mylocation.speedRight2);
        break;
    default:
        webSocket.broadcastTXT("Error: GetValue Unknown dataSource query");
        break;
    }
}

/**
 * The protocolSend function accepts 3 unsigned Chars and one int payload (4 Byte!). 
 * The function fits the data in the protocol structure and sends them out
 */
void protocolSend(unsigned char dataType, unsigned char dataSource, unsigned char dataSubSource, int payload){
    unsigned char toSend [10];
    
    toSend[0] = 0x11; // start byte
    toSend[1] = 0x01; // indicator "data from RDM53"
    toSend[2] = dataType;
    toSend[3] = dataSource;
    toSend[4] = dataSubSource;

    toSend[5] = payload >> 24;
    toSend[6] = payload >> 16;
    toSend[7] = payload >> 8;
    toSend[8] = payload;

    toSend[9] = 0x12;
    sendBinCharArr(toSend, 10);
}
/**
 * The protocolSend function accepts 3 unsigned chars and one float payload.
 * The function adds this data according to the protocol and calls sendBinCharArr.
 */
void protocolSend(unsigned char dataType, unsigned char dataSource, unsigned char dataSubSource, float payload){
    unsigned char toSend [10];
    union floatToBytes {
        char buffer[4];
        float number;
    } converter;
    
    toSend[0] = 0x11; // start byte
    toSend[1] = 0x01; // indicator "data from RDM53"
    toSend[2] = dataType;
    toSend[3] = dataSource;
    toSend[4] = dataSubSource;

    converter.number = payload;

    toSend[5] = converter.buffer[0];
    toSend[6] = converter.buffer[1];
    toSend[7] = converter.buffer[2];
    toSend[8] = converter.buffer[3];

    toSend[9] = 0x12;
 
    Serial.println(converter.buffer);
    Serial.println(converter.number);
    sendBinCharArr(toSend, 10);
}