#ifndef SPEEDSENSOR_H
#define SPEEDSENSOR_H

#include <Arduino.h>

float doMeasureLeft();
float doMeasureRight();
float measure(int, long);
void setHoleLeft(float);
void setHoleRight(float);

#endif