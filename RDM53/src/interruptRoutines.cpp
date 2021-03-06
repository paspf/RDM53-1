/*
 * This file contains all interrupt routines and their handling
 * 
 * Creation date: 2019 06 16
 * Author: Pascal Pfeiffer
 */

// includes
#include "interruptRoutines.h"
#include "PublicStructures.h"
#include "protocol.h"
#include "lidar.h"
#include "lineTracking.h"
#include "readSensors.h"
#include <HCSR04P.h>
#include <Arduino.h>
#include "colorTracking.h"
#include "location.h"

// extern objects
extern deviceConfig dC;
extern lidar lidarSensors;
extern lineTrackInterface lineSensorFrontLeft;
extern lineTrackInterface lineSensorFrontRight;
extern lineTrackInterface lineSensorBackLeft;
extern lineTrackInterface lineSensorBackRight;
extern HCSR04P ultraSonic;
extern ColTrack colTrack;
extern Location mylocation;

// iterrupts markers
volatile byte interruptCounterKey1 = 0;
volatile byte interruptCounterKey2 = 0;
volatile byte timer0State = 0;

// create a hardware timer
hw_timer_t * timer0 = NULL;

// Critical section management
portMUX_TYPE pinMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timer0Mux = portMUX_INITIALIZER_UNLOCKED;

// couter for 1 second
byte secCounter = 0;

/**
 * Interrupt Service Routine for Pin 19
 */
void IRAM_ATTR handleInterruptP19() {
  portENTER_CRITICAL_ISR(&pinMux);
  interruptCounterKey1++;
  portEXIT_CRITICAL_ISR(&pinMux);
}

/**
 * Interrupt Service Routine for Pin 18
 */
void IRAM_ATTR handleInterruptP18() {
  portENTER_CRITICAL_ISR(&pinMux);
  interruptCounterKey2++;
  portEXIT_CRITICAL_ISR(&pinMux);
}

/**
 * Timer0 ISR
 * ISR routine stored in IRAM
 */
void IRAM_ATTR onTimer0() {
  portENTER_CRITICAL(&timer0Mux);
  timer0State++;
  portEXIT_CRITICAL(&timer0Mux);
}

/**
 * Initialize Interrupts
 * Configure External Interrupt Pins and timer interrupts
 */
void interruptInitialization() {
  pinMode(KEY1, INPUT);
  pinMode(KEY2, INPUT);
  attachInterrupt(digitalPinToInterrupt(KEY1), handleInterruptP19, FALLING);
  attachInterrupt(digitalPinToInterrupt(KEY2), handleInterruptP18, FALLING);

  Serial.print("start timer0...");
  // init timer0, prescaler = 80, countup = true
  timer0 = timerBegin(0, 80, true);

  // interrupt timer0, function = onTimer0, edge = true
  timerAttachInterrupt(timer0, &onTimer0, true);

  // counter timer0, value = 200000, relead after interrupt = true
  // timer interrupts programm every 200ms
  timerAlarmWrite(timer0, 200000, true);

  // enable the timer
  timerAlarmEnable(timer0);
  Serial.println("[OK]");
}

/*
void keyHandler() {
    if(interruptCounterKey1 > 0){
      // disable interrupts on both CPU cores, and spinlock tasks on other CPU
      portENTER_CRITICAL(&pinMux);
      interruptCounterKey1 = 0;
      Serial.println("Key1 Pressed!");
      // enable interrupts on both CPU cores, disable spinlock
      portEXIT_CRITICAL(&pinMux);
  }

  if(interruptCounterKey2 > 0){
      // disable interrupts on both CPU cores, and spinlock tasks on other CPU
      portENTER_CRITICAL(&pinMux);
      Serial.println("Key2Pressed!");
      interruptCounterKey2 = 0;
      // enable interrupts on both CPU cores, disable spinlock
      portEXIT_CRITICAL(&pinMux);
  }
}
*/

/**
 * Do stuff that should be run if an Interrupt has occured
 * This function is OUTSIDE of the interrupt itself
 */
void interruptWorkers() {

  // keyHandler();

  // this block is triggered every 200ms
  if(timer0State >= 1) {
    portENTER_CRITICAL(&timer0Mux);
    timer0State = 0;
    portEXIT_CRITICAL(&timer0Mux);

    portENTER_CRITICAL_ISR(&pinMux);
    // interruptCounterKey1 = Left speed sensor, interruptCounterKey2 = right speed sensor;
    mylocation.calculateSpeed(interruptCounterKey1, interruptCounterKey2);
    interruptCounterKey1 = 0;
    interruptCounterKey2 = 0;
    portEXIT_CRITICAL_ISR(&pinMux);
    // HIER SENSOR ZYKLISCH AUSLESEN UND GRAD BERECHNEN
    
    secCounter++;
  }

  // 1s intervall
  if(secCounter >= 5) {
      if(dC.wiFiNotificationSender == true) {
        wiFiNotificationSender();
    }
    secCounter = 0;
  }
  yield();
}

/**
 * This function is continuously called if the
 * dC.wiFiNotificationSender value is true
 * The function sends all recent sensor values
 * to the websocket client
 */
void wiFiNotificationSender() {
  // line tracking Sensors
  // 11 03 03 11 00 00 00 00 00 12
  protocolSend(0x0, 0x11, 0x00,  lineSensorFrontLeft.getColorCode());
  protocolSend(0x0, 0x12, 0x00,  lineSensorFrontRight.getColorCode());
  protocolSend(0x0, 0x13, 0x00,  lineSensorBackLeft.getColorCode());
  protocolSend(0x0, 0x14, 0x00,  lineSensorBackRight.getColorCode());

  // lidar sensors
  protocolSend(0x0, 0x00, 0x00,  lidarSensors.measureLidar[0].RangeMilliMeter);
  protocolSend(0x0, 0x01, 0x00,  lidarSensors.measureLidar[1].RangeMilliMeter);
  protocolSend(0x0, 0x02, 0x00,  lidarSensors.measureLidar[2].RangeMilliMeter);
  protocolSend(0x0, 0x03, 0x00,  lidarSensors.measureLidar[3].RangeMilliMeter);
  protocolSend(0x0, 0x04, 0x00,  lidarSensors.measureLidar[4].RangeMilliMeter);
  protocolSend(0x0, 0x05, 0x00,  lidarSensors.measureLidar[5].RangeMilliMeter);
  protocolSend(0x0, 0x06, 0x00,  lidarSensors.measureLidar[6].RangeMilliMeter);

  // ultrasonic
  protocolSend(0x0, 0x08, 0x00,  ultraSonic.getDist());

  // combined speed
  protocolSend(0x0, 0x19, 0x00, (float) mylocation.speedCombined2);

}
