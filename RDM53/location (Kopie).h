/*
 * This file contains all functions for Kinemtatics and Localisation
 * It is based on the bolderflight MPU9250 Library https://github.com/bolderflight/MPU9250
 * as well as the quaternion filters from kriswiner: https://github.com/kriswiner/MPU9250/blob/master/quaternionFilters.ino 
 * Currently please so not use the Speed or Position functions as they have not been cleaned from the Gravity component.
 * 
 * Author: Jan Kühnemund
 */


#include <Arduino.h>

#ifndef LOCATION_H
    #define LOCATION_H

    class Location{
    public:
        float yaw = 0, pitch = 0, roll = 0;
        float speedX = 0, speedY = 0, speedZ = 0, speedGen = 0;
        float posX = 0, posY = 0, posZ = 0;
        int mXOffset = 26, mYOffset = -47, mZOffset = -31;
        
        void startMP();
        float getHeading();
        float getPitch();
        float getRoll();
        float getSpeedX();
        float getSpeedY();
        float getSpeedZ();
        float getSpeedGen();
        float getPosX();
        float getPosY();
        float getPosZ();

        float getAccX();
        float getAccY();
        float getAccZ();
        float getGyrX();
        float getGyrY();
        float getGyrZ();
        float getMagX();
        float getMagY();
        float getMagZ();
        
        void updateLocationVars();

    private:

        int SDA_PIN = 21, SCL_PIN = 22;
        // global constants for 9 DoF fusion and AHRS (Attitude and Heading Reference System)
        float GyroMeasError = PI * (40.0f / 180.0f);   // gyroscope measurement error in rads/s (start at 40 deg/s)
        float GyroMeasDrift = PI * (0.0f / 180.0f); // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)     
        float beta = sqrt(3.0f / 4.0f) * GyroMeasError;   // compute beta
        float zeta = sqrt(3.0f / 4.0f) * GyroMeasDrift;   // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value
        #define Kp 2.0f * 5.0f // these are the free parameters in the Mahony filter and fusion scheme, Kp for proportional feedback, Ki for integral
        #define Ki 0.0f
        
        float accx = 0, accy = 0, accz = 0, gyrx = 0, gyry = 0, gyrz = 0, magx = 0, magy = 0, magz = 0;
        float gravx = 0, gravy = 0, gravz = 0, c_accx = 0, c_accy = 0, c_accz = 0;
        float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};    // vector to hold quaternion
        float eInt[3] = {0.0f, 0.0f, 0.0f}; 
        
        float deltat = 0.0f, sum = 0.0f;        // integration interval for both filter schemes
        uint32_t lastUpdate = 0, firstUpdate = 0; // used to calculate integration interval
        uint32_t Now = 0; // used to calculate integration interval
        
        int status, sumCount;
        float srdVal = 19;
        float period = (1/(1+srdVal));
        
        bool SerialVal = false;
        bool SerialHeading = false;
        bool SerialSpeed = false;
        bool SerialPos = false;
    
        void MahonyQuaternionUpdate(float accx, float accy, float accz, float gyrx, float gyry, float gyrz, float magx, float magy, float magz);
        void MadgwickQuaternionUpdate(float accx, float accy, float accz, float gyrx, float gyry, float gyrz, float magx, float magy, float magz);
    };

#endif