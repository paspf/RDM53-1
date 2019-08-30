#ifndef SPEEDSENSOR_H
#define SPEEDSENSOR_H

#include <Arduino.h>

float doMeasureLeft();
float doMeasureRight();
float measure(float, long);
void setHoleLeft(float);
void setHoleRight(float);

#endif