#include <Arduino.h>


#ifndef OBSTACLEANDLINE_H
#define OBSTACLEANDLINE_H
    
    class ObstacleAndLine {


        public:


        void checkMod();
        int startMod1;
        int startMod2;
        void driveThroughParcour();
        unsigned short mod = 0;



    };
#endif