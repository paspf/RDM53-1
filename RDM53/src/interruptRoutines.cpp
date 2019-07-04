/*
 * This file contains all interrupt routines and their handling
 * 
 * Date: 2019 06 16
 * Author: Pascal Pfeiffer
 */
#include "interruptRoutines.h"

// Critical section management
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

/*
 * Interrupt Service Routine for Pin 19
 */
void IRAM_ATTR handleInterruptP19() {
  portENTER_CRITICAL_ISR(&mux);
  interruptCounterKey1++;
  portEXIT_CRITICAL_ISR(&mux);
}

/*
 * Interrupt Service Routine for Pin 18
 */
void IRAM_ATTR handleInterruptP18() {
  portENTER_CRITICAL_ISR(&mux);
  interruptCounterKey2++;
  portEXIT_CRITICAL_ISR(&mux);
}

/*
 * Initialize Interrupts
 * Configure External Interrupt Pins
 */
void interruptInitialization() {
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(KEY1), handleInterruptP19, FALLING);
  attachInterrupt(digitalPinToInterrupt(KEY2), handleInterruptP18, FALLING);
}

/*
 * Do stuff that should be run if an Interrupt has occured
 * This function is OUTSIDE of the interrupt itself
 */
void interruptWorkers() {
 
  if(interruptCounterKey1 > 0){
      // disable interrupts on both CPU cores, and spinlock tasks on other CPU
      portENTER_CRITICAL(&mux);
      interruptCounterKey1 = 0;
      // enable interrupts on both CPU cores, disable spinlock
      portEXIT_CRITICAL(&mux);

      //Serial.print("An KEY 1 1interrupt has occurred.\n");
  }
  if(interruptCounterKey2 > 0){
      // disable interrupts on both CPU cores, and spinlock tasks on other CPU
      portENTER_CRITICAL(&mux);
      interruptCounterKey2 = 0;
      // enable interrupts on both CPU cores, disable spinlock
      portEXIT_CRITICAL(&mux);

      //Serial.print("An KEY 2 1interrupt has occurred.\n");
  }
  yield();
}