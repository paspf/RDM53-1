#include <Arduino.h>


#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H
    class LineFollower {

        public:
        
        unsigned int cntleft = 0;
        unsigned short directionleft;
        unsigned int cntright = 0;
        unsigned short directionright;

        void followLine();

    };
#endif