/*
 * ObstacleAndLine combines the functionality of
 * LineFollower.cpp and ObstacleAvoidance.cpp
 * 
 * IR Sensors (line Tracking), Ultrasonic, Lidar and the color sensor is used
 * 
 * Creation date: 2019 08 02
 * Author: Taha Tekdemir
 */

#include <Arduino.h>

#ifndef OBSTACLEANDLINE_H
#define OBSTACLEANDLINE_H
    
    class ObstacleAndLine {
        public:
        int startMod69;
        int startMod70;
        int startMod10;
        int checkMod();
        int startMod1;
        int startMod2;
        int startMod13;
        void driveThroughParcour();
        unsigned short mod = 0;
        void initValues();
    };
#endif