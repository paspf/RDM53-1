#include <Arduino.h>


#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H
    class LineFollower {

        public:
        
        unsigned int cntleft;
        unsigned short directionleft;
        unsigned int cntright;
        unsigned short directionright;

        void followLine();


    };
#endif