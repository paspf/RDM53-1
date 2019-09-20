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