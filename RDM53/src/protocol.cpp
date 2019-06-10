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
            dC.mode = 0x00000 + payload;
            break;
        case 0x1: //RemoteControl
            //Start thread remotecontrol and break other threads.
            // enable remote control
            dc.mode = 0x10000 + payload;
            break;
        case 0x2:
            //Break Autonomy and RemoteControl. DeepSleepMode?
            dc.mode = 0x20000 + payload;
            break;
        case 0x3:
            //
            dc.mode = 0x30000 + payload;
            break;
        default:
            Serial.println("ERROR: protocolEnter: Undefined Mode set due to invalid incoming[2]");
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
            calibration(incoming);
            break;
        case 0x2: //Testing
            testing(incoming);
            break;
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
    int n;
    char buffer[128];
    switch (incoming[3])
        {
        case 0x0: // forward / backward
            //speed(strtol(payload);
            Serial.println("Vor/ Rueck!");
            break;
        case 0x1: // turn value
            //turnrate(strtol(payload);
            Serial.println("Turn Value!");
        default:
            break;
        }
    n = sprintf(buffer, "Remote ID %d would now have Value: %x", incoming[3], payload);
    Serial.println(buffer);
}

void calibration(unsigned char * incoming){
    int n;
    char buffer[64];

}

void testing(unsigned char* incoming){
    int payload = (incoming[5]<<24) | (incoming[6]<<16) | (incoming[7]<<8) | incoming[8];
    switch (incoming[3]){
        case 0x0:
            //motor1(payload);
            break;
        case 0x1:
            //motor2(payload);
            break;
        case 0x2:
            //motor3(payload);
            break;
        case 0x3:
            //motor4(payload);
            break;
        case 0x4:
            //piezo(payload);
            break;
        default :
            Serial.println("Error in testing() due to invalid incoming[3]");
            break;
    }
}
/*The protocolSend function accepts 3 unsigned Chars and one int payload (4 Byte!). 
 *These it prints serially in the form of our protocol. 
 */
void protocolSend(unsigned char dataType, unsigned char dataSource, unsigned char dataSubSource, int payload){
    unsigned char toSend [10];

    toSend[0] = 0x11;
    toSend[1] = 0x01;
    toSend[2] = dataType;
    toSend[3] = dataSource;
    toSend[4] = dataSubSource;
    for(int i = 5; i < 9; i++) {
        // tmp = payload && (0xFF << ((4-i)*8));
        toSend[i] = (unsigned char) payload >> ((9-i)*8);
    }
    toSend[10] = 0x12;

    for(int i = 0; i<11; i++){
        Serial.print(toSend[i]);
    }
    webSocket.broadcastBIN()
    webSocket
    //Serial.println("Protocol Send Test:");
    Serial.print(0x11);
    Serial.print(dataType);
    Serial.print(dataSource);
    Serial.print(axis);

    for(int i = 0; i < 4; i++) {
        Serial.print(tests[i]);
    }
    Serial.println(0x12);
    //Serial.println("END Protocol Send Test!");
    //tmp = sprintf("01%x%x%x%x", dataType, dataSource, mode, payload);
}