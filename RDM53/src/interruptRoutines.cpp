/*
 * This file contains all interrupt routines and their handling
 * 
 * Date: 2019 06 16
 * Author: Pascal Pfeiffer
 */
#include "interruptRoutines.h"
#include <Arduino.h>

// iterrupts markers
volatile byte interruptCounterKey1 = 0;
volatile byte interruptCounterKey2 = 0;
volatile byte timer0State = 0;

// create a hardware timer
hw_timer_t * timer0 = NULL;

// Critical section management
portMUX_TYPE pinMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timer0Mux = portMUX_INITIALIZER_UNLOCKED;

/*
 * Interrupt Service Routine for Pin 19
 */
void IRAM_ATTR handleInterruptP19() {
  portENTER_CRITICAL_ISR(&pinMux);
  interruptCounterKey1++;
  portEXIT_CRITICAL_ISR(&pinMux);
}

/*
 * Interrupt Service Routine for Pin 18
 */
void IRAM_ATTR handleInterruptP18() {
  portENTER_CRITICAL_ISR(&pinMux);
  interruptCounterKey2++;
  portEXIT_CRITICAL_ISR(&pinMux);
}

/*
 * Timer0 ISR
 * ISR routine stored in IRAM
 */
void IRAM_ATTR onTimer0() {
  portENTER_CRITICAL(&timer0Mux);
  timer0State++;
  portEXIT_CRITICAL(&timer0Mux);
}

/*
 * Initialize Interrupts
 * Configure External Interrupt Pins and timer interrupts
 */
void interruptInitialization() {
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(KEY1), handleInterruptP19, FALLING);
  attachInterrupt(digitalPinToInterrupt(KEY2), handleInterruptP18, FALLING);

  Serial.print("start timer0...");
    // init timer0, prescaler = 80, countup = true
  timer0 = timerBegin(0, 80, true);

  // interrupt timer0, function = onTimer0, edge = true
  timerAttachInterrupt(timer0, &onTimer0, true);

  // counter timer0, value = 100000, relead after interrupt = true
  // timer interrupts programm every 100ms
  timerAlarmWrite(timer0, 100000, true);

  // enable the timer
  timerAlarmEnable(timer0);
  Serial.println("[OK]");
}

/*
 * Do stuff that should be run if an Interrupt has occured
 * This function is OUTSIDE of the interrupt itself
 */
void interruptWorkers() {
 
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
  
  if(timer0State >= 1) {
    // Serial.print("TIM0: ");
    // Serial.println(millis());
    portENTER_CRITICAL(&timer0Mux);
    timer0State = 0;
    portEXIT_CRITICAL(&timer0Mux);
  }
  yield();
}