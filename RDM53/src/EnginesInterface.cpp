/*
 * This file contains the engines interface
 * using the ESP32 LEDPWM, DRV8833 and some DC engines
 * 
 * Date: 2019 06 12
 * Author: Pascal Pfeiffer
 */

#include "EnginesInterface.h"

/**
 * The constructor performs basic setup functions
 * - set PWM frequency and resolution
 * - attach pins to ledpwm
 */
EnginesInterface::EnginesInterface() {
    pinMode(EFL1_PIN,OUTPUT);
    pinMode(EFL2_PIN,OUTPUT);
    pinMode(EFR1_PIN,OUTPUT);
    pinMode(EFR2_PIN,OUTPUT);
    pinMode(EBL1_PIN,OUTPUT);
    pinMode(EBL2_PIN,OUTPUT);
    pinMode(EBR1_PIN,OUTPUT);
    pinMode(EBR2_PIN,OUTPUT);

    ledcSetup(EFL1_PWM_CH, LED_PWM_FREQUENCY, LED_PWM_RESOLTION);
    ledcAttachPin(EFL1_PIN, EFL1_PWM_CH);

    ledcSetup(EFL2_PWM_CH, LED_PWM_FREQUENCY, LED_PWM_RESOLTION);
    ledcAttachPin(EFL2_PIN, EFL2_PWM_CH);

    ledcSetup(EFR1_PWM_CH, LED_PWM_FREQUENCY, LED_PWM_RESOLTION);
    ledcAttachPin(EFR1_PIN, EFR1_PWM_CH);

    ledcSetup(EFR2_PWM_CH, LED_PWM_FREQUENCY, LED_PWM_RESOLTION);
    ledcAttachPin(EFR2_PIN, EFR2_PWM_CH);

    ledcSetup(EBL1_PWM_CH, LED_PWM_FREQUENCY, LED_PWM_RESOLTION);
    ledcAttachPin(EBL1_PIN, EBL1_PWM_CH);

    ledcSetup(EBL2_PWM_CH, LED_PWM_FREQUENCY, LED_PWM_RESOLTION);
    ledcAttachPin(EBL2_PIN, EBL2_PWM_CH);

    ledcSetup(EBR1_PWM_CH, LED_PWM_FREQUENCY, LED_PWM_RESOLTION);
    ledcAttachPin(EBR1_PIN, EBR1_PWM_CH);

    ledcSetup(EBR2_PWM_CH, LED_PWM_FREQUENCY, LED_PWM_RESOLTION);
    ledcAttachPin(EBR2_PIN, EBR2_PWM_CH);
}

/**
 * Set a value to the engine front left
 * @param dir 0 - forward, 1 - backward
 * @param value pwm value from 0 to 255
 */
void EnginesInterface::setEFL(bool dir, unsigned char value) {
  if(dir == 0) {
    ledcWrite(EFL2_PWM_CH, 0);
    ledcWrite(EFL1_PWM_CH, value);
  }
  else {
    ledcWrite(EFL1_PWM_CH, 0);
    ledcWrite(EFL2_PWM_CH, value);
  }
}

/**
 * Set a value to the engine front right
 * @param dir 0 - forward, 1 - backward
 * @param value pwm value from 0 to 255
 */
void EnginesInterface::setEFR(bool dir, unsigned char value) {
  if(dir == 0) {
    ledcWrite(EFR2_PWM_CH, 0);
    ledcWrite(EFR1_PWM_CH, value);
  }
  else {
    ledcWrite(EFR1_PWM_CH, 0);
    ledcWrite(EFR2_PWM_CH, value);
  }
}

/**
 * Set a value to the engine back left
 * @param dir 0 - forward, 1 - backward
 * @param value pwm value from 0 to 255
 */
void EnginesInterface::setEBL(bool dir, unsigned char value) {
  if(dir == 0) {
    ledcWrite(EBL2_PWM_CH, 0);
    ledcWrite(EBL1_PWM_CH, value);
  }
  else {
    ledcWrite(EBL1_PWM_CH, 0);
    ledcWrite(EBL2_PWM_CH, value);
  }
}

/**
 * Set a value to the engine back right
 * @param dir 0 - forward, 1 - backward
 * @param value pwm value from 0 to 255
 */
void EnginesInterface::setEBR(bool dir, unsigned char value) {
  if(dir == 0) {
    ledcWrite(EBR2_PWM_CH, 0);
    ledcWrite(EBR1_PWM_CH, value);
  }
  else {
    ledcWrite(EBR1_PWM_CH, 0);
    ledcWrite(EBR2_PWM_CH, value);
  }
}

/*
 * Stop all engines
 */
void EnginesInterface::stopE() {
  ledcWrite(EFL1_PWM_CH, 0);
  ledcWrite(EFL2_PWM_CH, 0);
  ledcWrite(EFR1_PWM_CH, 0);
  ledcWrite(EFR2_PWM_CH, 0);
  ledcWrite(EBL1_PWM_CH, 0);
  ledcWrite(EBL2_PWM_CH, 0);
  ledcWrite(EBR1_PWM_CH, 0);
  ledcWrite(EBR2_PWM_CH, 0);
}