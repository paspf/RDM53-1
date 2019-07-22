#ifndef AHRS
    #define AHRS
    #include "MPU9250.h"
    #include <MadgwickAHRS.h>
    #include <math.h>
    #include <connectivity.h>

    class AHRSInterface{
        private:
        Madgwick filter;
        MPU9250 IMU;
        unsigned long microsPerReading, microsPrevious, microsNow;
        float accelScale, gyroScale;
        int aix, aiy, aiz;
        int gix, giy, giz;
        float ax, ay, az;
        float gx, gy, gz;
        float mx,  my,  mz;
        float roll, pitch, heading;

        public:
        int initAHRS();
        int setAHRSrange(uint AccRange, uint GyrRange, uint MagBandwidth);
        void setAccCal(float xBias, float xScale, float yBias, float yScale, float zBias, float zScale);
        void setGyroCal(float xBias, float yBias, float zBias);
        void setMagCal(float xBias, float xScale, float yBias, float yScale, float zBias, float zScale);
        int calAccGyro();
        int calFull();

        void updateFilter();
        void updateVal();

        float getAccX();
        float getAccY();
        float getAccZ();

        float getGyrX();
        float getGyrY();
        float getGyrZ();

        float getMagX();
        float getMagY();
        float getMagZ();

        float getHeading();
        float getPitch();
        float getRoll();
    };

#endif