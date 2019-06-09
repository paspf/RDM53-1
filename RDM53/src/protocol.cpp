/*
 *This file contains all the protocol functions for the ESP32
 *
 *
 *Author: Jan Kühnemund
 */

#include <protocol.h>


int protocolEnter(unsigned char* incoming, size_t length)
{
    int payload = 0;
    Serial.println("Payload JAN:");
    Serial.println(incoming[5], HEX);
    Serial.println(incoming[6], HEX);
    Serial.println(incoming[7], HEX);
    Serial.println(incoming[8], HEX);
    Serial.println("END PAYLOAD JAN");

    payload = (incoming[5]<<24) | (incoming[6]<<16) | (incoming[7]<<8) | incoming[8];
    Serial.println(payload, HEX);

    if(incoming[1]== 2) //switchMode
    {
        // Select device mode
        switch (incoming[2])
        {
        case 0x0: //Autonomous
            //Start thread autonomy and breaking all other threads.
            // payload == autonomous mode
            autonomous(payload);
            // dC.mode = x;
            break;
        case 0x1: //RemoteControl
            //Start thread remotecontrol and break other threads.
            // enalbe remote control
        case 0x2:
            //Break Autonomy and RemoteControl. DeepSleepMode?
        case 0x3:
            //
        default:
            break;
        }
    }
    if(incoming[1] == 3){ //receiveData
        switch (incoming[2])
        {
        case 0x0: //RemoteControlData
            remoteControl(incoming);
            break;
        case 0x1: //Calibration
            calibration(incoming[2], payload);
        case 0x2: //Testing
            testing(incoming[2], payload);
        default:
            break;
        }
    }
    return NULL;
}

void autonomous(int autonomyNum)
{ 
    //Put functions leading to Autonomy here
    int n;
    char buffer[128];
    n = sprintf(buffer, "Autonomy mode %x will now commence", autonomyNum);
    Serial.println(buffer);
}

void remoteControl(unsigned char* incoming)
{
    int payload = (incoming[5]<<24) | (incoming[6]<<16) | (incoming[7]<<8) | incoming[8];
    //speed(strtol(payload, NULL, 16) / 0xFF);
    //turnrate(strtol(payload, NULL, 16) % 0xFF);
    int n;
    char buffer[128];
    switch (incoming[3])
        {
        case 0x0: // forward / backward
            Serial.println("Vor/ Rueck!");
            break;
        case 0x1: // turn value
            Serial.println("Turn Value!");
        default:
            break;
        }
    n = sprintf(buffer, "Remote ID %d would now have Value: %x", incoming[3], payload);
    Serial.println(buffer);
}

void calibration(char ID, int value){
    int n;
    char buffer[64];
    n = sprintf(buffer, "Calibration ID %c would now have Calibration Value: %x", ID, value);
}

void testing(char testID, int value){
    int n;
    char buffer[64];
    n = sprintf(buffer, "Testing %c with value %x", testID, value);
}

void protocolSend(unsigned char dataType, unsigned char dataSource, unsigned char mode, int payload){
    unsigned char tests [4];
    int tmp;
    for(int i = 0; i < 4; i++) {
        // tmp = payload && (0xFF << ((4-i)*8));
        tests[i] = (unsigned char) payload >> ((4-i)*8);
    }
    Serial.println("Protocol Send Test:");
    for(int i = 0; i < 4; i++) {
        Serial.println(tests[i]);
    }
    Serial.println("END Protocol Send Test!");
    // tmp = sprintf("01%x%x%x%x", dataType, dataSource, mode, payload);
}