/*
 * This file contains all header informations for the piezo speaker
 * 
 * Creation date: 2019 07 02
 * Author: Pascal Pfeiffer
 */


#include <Arduino.h>

#ifndef PIEZO_INTERFACE
#define PIEZO_INTERFACE

#define PIEZO_PIN 13
#define PIEZO_PWM_CH 8

#define LED_PWM_PIEZO_FREQUENCY 1000
#define LED_PWM_PIEZO_RESULTION 8

class PiezoInterface {
    public:
    PiezoInterface();
    void setPiezo(int freq, int vol);
    void setPiezo(int freq);
    void noSound();
};

#endif
