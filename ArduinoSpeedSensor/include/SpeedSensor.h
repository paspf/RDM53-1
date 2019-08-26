#ifndef SPEEDSENSOR_H
#define SPEEDSENSOR_H

#include <Arduino.h>

float doMeasureLeft();
float doMeasureRight();
void setHoleLeft(float);
void setHoleRight(float);

#endif