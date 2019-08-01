/*
 * This file contains all header informations for the engines interface
 * using the ESP32 LEDPWM, DRV8833 and some DC engines
 * 
 * Date: 2019 06 12
 * Author: Pascal Pfeiffer
 */

#ifndef ENGINES_INTERFACE_H
#define ENGINES_INTERFACE_H

#include <Arduino.h>

// Engine front left
#define EFL1_PIN 14
#define EFL1_PWM_CH 0

#define EFL2_PIN 12
#define EFL2_PWM_CH 1

// Engine front right
#define EFR1_PIN 16
#define EFR1_PWM_CH 2

#define EFR2_PIN 17
#define EFR2_PWM_CH 3

// Engine back left
#define EBL1_PIN 26
#define EBL1_PWM_CH 4

#define EBL2_PIN 27
#define EBL2_PWM_CH 5

// Engine back right
#define EBR1_PIN 4
#define EBR1_PWM_CH 6

#define EBR2_PIN 33
#define EBR2_PWM_CH 7

// general PWM
#define LED_PWM_FREQUENCY 1000
#define LED_PWM_RESOLTION 8

/*
* Engines Interface
*/
class EnginesInterface {
    public:
    EnginesInterface();
    void setEFL(bool, unsigned char);
    void setEFR(bool, unsigned char);
    void setEBL(bool, unsigned char);
    void setEBR(bool, unsigned char);
    void stopE();
};

#endif