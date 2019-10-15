/*
 * ObstacleAndLineRNGLAB combines the functionality of
 * LineFollower.cpp, ObstacleAvoidance.cpp, ObstacleAndLine.cpp
 * and some optimazions for the RNG LAB like improved goal detection
 * and control functions
 * 
 * IR Sensors (line Tracking), Ultrasonic, Lidar and the color sensor is used
 * 
 * Creation date: 2019 09 19
 * Author: Pascal Pfeiffer, Taha Tekdemir
 */

#include <Arduino.h>


#ifndef OBSTACLEANDLINE_RNGLAB_H
#define OBSTACLEANDLINE_RNGLAB_H
    
    class ObstacleAndLine_RNGLAB {


        public:

        int checkMod();
        int startMod1;
        int startMod2;
        int startMod10;
        int startMod13;
        int startMod69;
        int startMod70;

        void driveThroughParcour();
        unsigned short mod = 0; // drive actions
        unsigned short modPasssive = 0; // passive actions, like goal found or something
        void initValues();
    };
#endif