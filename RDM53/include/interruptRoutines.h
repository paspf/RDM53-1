/*
 * This file contains all headin informations
 * for interrupt routines and their handling
 * 
 * Date: 2019 06 16
 * Author: Pascal Pfeiffer
 */

#include <Arduino.h>

#ifndef RDM_MAIN
    #define KEY1 19
    #define KEY2 18
    volatile int interruptCounterKey1 = 0;
    volatile int interruptCounterKey2 = 0;
#endif

void IRAM_ATTR handleInterruptP18();
void IRAM_ATTR handleInterruptP19();
void interruptInitialization();
void interruptWorkers();
