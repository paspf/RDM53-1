/*
 * This file contains all function implementations for the piezo speaker
 * 
 * Date: 2019 07 02
 * Author: Pascal Pfeiffer
 */

// includes
#include "piezo.h"


PiezoInterface::PiezoInterface() {
    pinMode(PIEZO_PIN,OUTPUT);
    ledcSetup(PIEZO_PWM_CH, LED_PWM_PIEZO_RESULTION, LED_PWM_PIEZO_RESULTION);
    ledcAttachPin(PIEZO_PIN, PIEZO_PWM_CH);
}

void PiezoInterface::setPiezo(int freq, int vol) {
    ledcSetup(PIEZO_PWM_CH, constrain(freq, 200, 2000), LED_PWM_PIEZO_RESULTION);
    ledcWrite(PIEZO_PWM_CH, constrain(vol, 0, 255));
}

void PiezoInterface::setPiezo(int freq) {
    ledcSetup(PIEZO_PWM_CH, constrain(freq, 200, 2000), LED_PWM_PIEZO_RESULTION);
    ledcWrite(PIEZO_PWM_CH, 10);
}

void PiezoInterface::noSound() {
    ledcWrite(PIEZO_PWM_CH, 0);
}