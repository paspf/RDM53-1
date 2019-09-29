/*
 * This file contains all headin informations
 * for interrupt routines and their handling
 * 
 * Creation date: 2019 06 16
 * Author: Pascal Pfeiffer
 */

#include <Arduino.h>

#ifndef INTERRUPT_ROUTINES_H
#define INTERRUPT_ROUTINES_H

    #define KEY1 19 // 19 -> left speed sensor
    #define KEY2 18  // 18 -> right speed sensor

    void IRAM_ATTR handleInterruptP18();
    void IRAM_ATTR handleInterruptP19();
    void interruptInitialization();
    void interruptWorkers();
    void IRAM_ATTR onTimer0();
    void wiFiNotificationSender();
    void calculateSpeed();
#endif