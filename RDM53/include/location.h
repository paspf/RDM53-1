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
    float yaw, pitch, roll;
    float speedX, speedY, speedGen, speedTrue, speedLeft, speedRight;
    float posX, posY;

    float wheelSize = 0.36;

    float get_mx_offset();
    float get_my_offset();
    float get_mz_offset();
    void set_mx_offset(float);
    void set_my_offset(float);
    void set_mz_offset(float);

    float get_avg_scale();
    void set_avg_scale(float);
    
    int startMP();
    float getHeading();
    float getPitch();
    float getRoll();
    float getSpeedX();
    float getSpeedY();
    float getSpeedTrue();
    float getSpeedLeft();
    float getSpeedRight();
    float getPosX();
    float getPosY();

    float getAccX();
    float getAccY();
    float getAccZ();
    float getGyrX();
    float getGyrY();
    float getGyrZ();
    float getMagX();
    float getMagY();
    float getMagZ();
    
    bool calibrate();
    void updateLocationVars();

    void aLittleJoke();


    void calculateSpeed(byte, byte);
    double speedFormula(double);
    double speedLeft2 = 0.0;
    double speedRight2 = 0.0;
    double speedCombined2 = 0.0;

private:
    // global constants for 9 DoF fusion and AHRS (Attitude and Heading Reference System)
    float GyroMeasError = PI * (40.0f / 180.0f);   // gyroscope measurement error in rads/s (start at 40 deg/s)
    float GyroMeasDrift = PI * (0.0f / 180.0f); // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)     
    float beta = sqrt(3.0f / 4.0f) * GyroMeasError;   // compute beta
    float zeta = sqrt(3.0f / 4.0f) * GyroMeasDrift;   // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value
    #define Kp 2.0f * 5.0f // these are the free parameters in the Mahony filter and fusion scheme, Kp for proportional feedback, Ki for integral
    #define Ki 0.0f
    
    float accx, accy, accz, gyrx, gyry, gyrz, magx, magy, magz;
    float gravx, gravy, gravz, c_accx, c_accy, c_accz;
    float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};    // vector to hold quaternion
    float eInt[3] = {0.0f, 0.0f, 0.0f}; 

    float max_x, max_y, max_z, min_x, min_y, min_z, mx_offset, my_offset, mz_offset, avg_scale;
    
    float deltat = 0.0f, sum = 0.0f;        // integration interval for both filter schemes
    uint32_t lastUpdate = 0, firstUpdate = 0; // used to calculate integration interval
    uint32_t Now = 0; // used to calculate integration interval
    
    int status, sumCount, lastLocUpdate;
    float srdVal = 19;
    float period = (1/(1+srdVal));
    
    bool SerialVal = false;
    bool SerialHeading = false;
    bool SerialSpeed = false;
    bool SerialPos = false;
  
    void MahonyQuaternionUpdate(float accx, float accy, float accz, float gyrx, float gyry, float gyrz, float magx, float magy, float magz);
    void MadgwickQuaternionUpdate(float accx, float accy, float accz, float gyrx, float gyry, float gyrz, float magx, float magy, float magz);
    
    #define SpeedSensor 0x70
    float i2cGetSpeed();
    float i2cGetSpeedLeft();
    float i2cGetSpeedRight();
    union floatToBytes {
        char buffer[4];
        float number;
    } converter;
    //location();
};

#endif