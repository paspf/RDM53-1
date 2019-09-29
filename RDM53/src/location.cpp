
// includes
#include "location.h"
#include "MPU9250.h"
#include "connectivity.h"
#include <math.h>
#include <string>
#include<stdio.h>

MPU9250 SensorArray(Wire1, 0x68);

// defines
// #define DEBUG_LOCATION

/** 
 * This function starts the sensor and sets all required components. If it has not been used
 * the sensor will not start. 
 */
int Location::startMP()
{
  if ((status = SensorArray.begin()) < 0) {
    //Serial.println("IMU initialization unsuccessful");
    //Serial.println("Check IMU wiring or try cycling power");
    //Serial.print("Status: ");
    //Serial.println(status);
    return status;
  }
  //Serial.println("IMU has started.");
  // setting DLPF bandwidth to 20 Hz
  SensorArray.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  // setting SRD to 19 for a 50 Hz update rate
  SensorArray.setSrd(srdVal);
  // enabling the data ready interrupt
  //IMU.enableDataReadyInterrupt();
  // attaching the interrupt to microcontroller pin 1
  //pinMode(23,INPUT);
  //attachInterrupt(23,getIMU,RISING);

  return status;

}

bool Location::calibrate()
{
  sendString("Do not move RDM. Calibrating Accelerometer and Gyroscope...");
  if(SensorArray.calibrateAccel() == false) {return false;}
  if(SensorArray.calibrateGyro() == false) {return false;}
  sendString("Move RDM in Loopsided 8 for Calibration of Magnetometer");
  sendString("This may take up to 20 seconds...");
  if(SensorArray.calibrateMag() == false) {return false;}
  sendString("Manual Magnetometer Calibration");

  for(int i = 0; i < 100; i++)
  {
    this->updateLocationVars();
    if(magx > max_x){max_x = magx;}
    if(magy > max_y){max_y = magy;}
    if(magz > max_z){max_z = magz;}
    if(magx < min_x){min_x = magx;}
    if(magy < min_y){min_y = magy;}
    if(magz < min_z){min_z = magz;}
    delay(50);
  }
  mx_offset = (max_x + min_x)/2;
  my_offset = (max_y + min_y)/2;
  mz_offset = (max_z + min_z)/2;

  float avg_delta_x = (max_x - min_x)/2;
  float avg_delta_y = (max_y - min_y)/2;
  float avg_delta_z = (max_z - min_z)/2;

  avg_delta = ((avg_delta_x + avg_delta_y + avg_delta_z)/3);

  avg_scale_x = avg_delta / avg_delta_x;
  avg_scale_y = avg_delta / avg_delta_y;
  avg_scale_z = avg_delta / avg_delta_z;

  sendString("Done");
  return true;
}
/** 
 * This function updates all variables.
 */
void Location::updateLocationVars()
{ 
    // Serial.println("Updating Loc Vars");       
    SensorArray.readSensor();
    
    gyrx = SensorArray.getGyroX_rads();
    gyry = SensorArray.getGyroY_rads();
    gyrz = SensorArray.getGyroZ_rads();
    accx = SensorArray.getAccelX_mss();
    accy = SensorArray.getAccelY_mss();
    accz = SensorArray.getAccelZ_mss();
    magx = (SensorArray.getMagX_uT()+mx_offset) * avg_scale_x;
    magy = (SensorArray.getMagY_uT()+my_offset) * avg_scale_y;
    magz = (SensorArray.getMagZ_uT()+mz_offset) * avg_scale_z;

    Now = micros();
    deltat = ((Now - lastUpdate)/1000000.0f); // set integration time by time elapsed since last filter update
    lastUpdate = Now;

    sum += deltat; // sum for averaging filter update rate
    sumCount++;
    
    MahonyQuaternionUpdate(accx/10, accy/10, accz/10, gyrx*PI/180.0f, gyry*PI/180.0f, gyrz*PI/180.0f,  10*magy,  10*magx, 10*magz);

    yaw   = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);   
    pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
    roll  = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]); 
    
    pitch *= 180.0f / PI;
    yaw   *= 180.0f / PI; 
    yaw   += 2.6; // Declination at Friedberg, Hessen, Germany is 2 degrees 36 minutes on 2019-07-31
    roll *= 180.0f / PI;

    gravx = 2 * (q[1] * q[3] - q[0] * q[2]);
    gravy = 2 * (q[0] * q[1] + q[2] * q[3]);
    gravz = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];

    c_accx = accx -gravx;
    c_accy = accy -gravy;
    c_accz = accz -gravz;

    speedTrue = i2cGetSpeed();
    speedLeft = i2cGetSpeedLeft();
    speedRight = i2cGetSpeedRight();

    speedX = speedTrue * cos(yaw);
    speedY = speedTrue * sin(yaw);
    
    posX = posX + speedX * millis() - lastLocUpdate;
    posY = posY + speedY * millis() - lastLocUpdate;
    lastLocUpdate=millis();

  }

/**
 * This function is called every 200ms
 * radius wheel: 3.1cm = 0.031m
 * 20 Segments per 360° -> 18° per segment
 */
  void Location::calculateSpeed(byte leftSegments, byte rightSegments) {
    #ifdef DEBUG_LOCATION
    Serial.println("CalculcateSpeed-----------------------");
    Serial.print("left: ");
    Serial.print(leftSegments);
    Serial.print("   right:");
    Serial.println(rightSegments);
    #endif

    speedLeft2 = speedFormula(leftSegments * 18);
    speedRight2 = speedFormula(rightSegments * 18);

    #ifdef DEBUG_LOCATION
    Serial.print("speedLeft: ");
    Serial.println(speedLeft2);
    #endif

    speedCombined2 = (speedLeft2 + speedRight2) / 2.0;
    #ifdef DEBUG_LOCATION
    Serial.println("END-------CalculcateSpeed------------");
    #endif
  }

/**
 * Formula to calculate arc of a circle
 */
  double Location::speedFormula(double degrees) {
    return (degrees / 72.0) * PI * 0.062;
  }

/** 
 * This function gets the current average rotational Speed of both sides of the Car. 
 * It is measured in rad/s. This number is always positive, even if you drive backwards.
 * 
 */
float Location::i2cGetSpeed()
{
    converter.number = 0;
    Wire1.beginTransmission(SpeedSensor);
    Wire1.write(0xA);
    Wire1.endTransmission();

    int index = 0;

    Wire1.requestFrom(SpeedSensor, 4);
    // Wait for response
    while (Wire1.available()) {
        byte b = Wire1.read();
        // Serial.print("i2cGetSpeed while loop: ");
        // Serial.println(b, HEX);
        converter.buffer[index] = b;
        index++;
    }
    // Serial.println(converter.number);
    return converter.number * wheelSize;   
}

/** 
 * This function gets the current average rotational Speed of both sides of the Car. 
 * It is measured in rad/s. This number is always positive, even if you drive backwards.
 * 
 */
float Location::i2cGetSpeedLeft()
{
    converter.number = 0;
    Wire1.beginTransmission(SpeedSensor);
    Wire1.write(0xB);
    Wire1.endTransmission();

    int index = 0;

    Wire1.requestFrom(SpeedSensor, 4);
    // Wait for response
    while (Wire1.available()) {
        byte b = Wire1.read();

        converter.buffer[index] = b;
        index++;
    }
    // Serial.println(converter.number);
    return converter.number * wheelSize;   
}

/** 
 * This function gets the current average rotational Speed of both sides of the Car. 
 * It is measured in rad/s. This number is always positive, even if you drive backwards.
 * 
 */
float Location::i2cGetSpeedRight()
{
    converter.number = 0;
    Wire1.beginTransmission(SpeedSensor);
    Wire1.write(0xC);
    Wire1.endTransmission();

    int index = 0;

    Wire1.requestFrom(SpeedSensor, 4);
    // Wait for response
    while (Wire1.available()) {
        byte b = Wire1.read();

        converter.buffer[index] = b;
        index++;
    }
    // Serial.println(converter.number);
    return converter.number * wheelSize;   
}
/** 
 * This file gets you the heading with 0 being North. It is measured in Degrees.
 * Turning clockwise gets you a higher number, turning counterclockwise a lower.
 * When facing south the number switches from -180 to 180 and vice versa.
 * 
 */
float Location::getHeading()
{
    return yaw;
}
/**
 *  This file gets you the Pitch. It is measured in Degrees.
 * Testing is required to see which direction yields a higher or lower number.
 */
float Location::getPitch()
{
    return pitch;
}
/**
 *  This file gets the Roll. It is measured in Degrees.
 * Testing is required to see which direction yields a higher or lower number.
 */
float Location::getRoll()
{
    return roll;
}
/**
 *  This function gets the Speed of the sensor in the x-direction (forward)
 * The speed is measured in m/s (meters per second)
 */
float Location::getSpeedX(){
  return speedX;
}
/**
 *  This function gets the Speed of the sensor in the y-direction (sideways)
 * The speed is measured in m/s (meters per second)
 */
float Location::getSpeedY(){
  return speedY;
}
/**
 *  This function gets the Speed of the wheels in general 
 * The speed is measured in m/s (meters per second)
 * this value is always positive and doesn't tell you anything about the direction
 */
float Location::getSpeedTrue(){
  Serial.println(speedTrue);
  return speedTrue;
}

float Location::getSpeedLeft(){
  Serial.println(speedLeft);
  return speedLeft;
}

float Location::getSpeedRight(){
  Serial.println(speedRight);
  return speedRight;
}
/**
 *  This function gets the distance travelled in the x-axis (forward/backward)
 * The distance is measured in m
 */
float Location::getPosX(){
  return posX;
}
/**
 *  This function gets the distance travelled in the y-axis (sideways)
 * The distance is measured in m
 */
float Location::getPosY(){
  return posY;
}

/**
 *  This function gets the accelaration on the x-axis (forward/backward)
 * The accelaration is measured in m/s²
 * Be aware that the gravity component has not been removed
 */
float Location::getAccX(){
  return accx;
}
/**
 *  This function gets the accelaration on the y-axis (sideways)
 * The accelaration is measured in m/s²
 * Be aware that the gravity component has not been removed
 */
float Location::getAccY(){
  return accy;
}
/**
 *  This function gets the accelaration on the z-axis (up and down)
 * The accelaration is measured in m/s²
 * Be aware that the gravity component has not been removed
 */
float Location::getAccZ(){
  return accz;
}
/**
 *  This function gets the rotation around the x-axis (forward/backward)
 * The rotation is measured in degrees/s
 */
float Location::getGyrX(){
  return gyrx;
}
/**
 *  This function gets the rotation around the y-axis (sideways)
 * The rotation is measured in rad/s
 */
float Location::getGyrY(){
  return gyry;
}
/**
 *  This function gets the rotation around the z-axis (up and down)
 * The rotation is measured in rad/s
 */
float Location::getGyrZ(){
  return gyrz;
}
/**
 *  This function gets the magnetic induction at the x-axis (forward/backward)
 * The rotation is measured in uT (Micro Tesla), T * 10^-6
 */
float Location::getMagX(){
  return magx;
}
/**
 * This function gets the magnetic induction at the y-axis (sideways)
 * The rotation is measured in uT (Micro Tesla), T * 10^-6
 */
float Location::getMagY(){
  return magy;
}
/**
 * This function gets the magnetic induction at the z-axis (up and down)
 * The rotation is measured in uT (Micro Tesla), T * 10^-6
 */
float Location::getMagZ(){
  return magz;
}

float Location::get_mx_offset()
{
  return mx_offset;
}

float Location::get_my_offset()
{
  return my_offset;
}

float Location::get_mz_offset()
{
  return mz_offset;
}

float Location::get_mx_scale()
{
  return avg_scale_x;
}

float Location::get_my_scale()
{
  return avg_scale_y;
}

float Location::get_mz_scale()
{
  return avg_scale_z;
}

float Location::get_avg_scale()
{
  return avg_delta;
}

float Location::get_Ki()
{
  return Ki;
}

float Location::get_Kp()
{
  return Kp;
}

void Location::set_mx_offset(float var)
{
  mx_offset = var;
}

void Location::set_my_offset(float var)
{
  my_offset = var;
}

void Location::set_mz_offset(float var)
{
  mz_offset = var;
}

void Location::set_avg_scale(float var)
{
  avg_delta = var;
}

void Location::set_mx_scale(float var)
{
  avg_scale_x = var;
}

void Location::set_my_scale(float var)
{
  avg_scale_y = var;
}

void Location::set_mz_scale(float var)
{
  avg_scale_z = var;
}

void Location::set_Kp(float var)
{
  Kp = var;
}

void Location::set_Ki(float var)
{
  Ki = var;
}

/**
 * Implementation of Sebastian Madgwick's "...efficient orientation filter for... inertial/magnetic sensor arrays"
 * (see http://www.x-io.co.uk/category/open-source/ for examples and more details)
 * which fuses acceleration, rotation rate, and magnetic moments to produce a quaternion-based estimate of absolute
 * device orientation -- which can be converted to yaw, pitch, and roll. Useful for stabilizing quadcopters, etc.
 * The performance of the orientation filter is at least as good as conventional Kalman-based filtering algorithms
 * but is much less computationally intensive---it can be performed on a 3.3 V Pro Mini operating at 8 MHz!
 * Source: https://github.com/kriswiner/MPU9250/blob/master/quaternionFilters.ino  
 */     
        
void Location::MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz)
{
    float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];   // short name local variable for readability
    float norm;
    float hx, hy, _2bx, _2bz;
    float s1, s2, s3, s4;
    float qDot1, qDot2, qDot3, qDot4;

    // Auxiliary variables to avoid repeated arithmetic
    float _2q1mx;
    float _2q1my;
    float _2q1mz;
    float _2q2mx;
    float _4bx;
    float _4bz;
    float _2q1 = 2.0f * q1;
    float _2q2 = 2.0f * q2;
    float _2q3 = 2.0f * q3;
    float _2q4 = 2.0f * q4;
    float _2q1q3 = 2.0f * q1 * q3;
    float _2q3q4 = 2.0f * q3 * q4;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q1q4 = q1 * q4;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q2q4 = q2 * q4;
    float q3q3 = q3 * q3;
    float q3q4 = q3 * q4;
    float q4q4 = q4 * q4;

    // Normalise accelerometer measurement
    norm = sqrtf(ax * ax + ay * ay + az * az);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f/norm;
    ax *= norm;
    ay *= norm;
    az *= norm;

    // Normalise magnetometer measurement
    norm = sqrtf(mx * mx + my * my + mz * mz);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f/norm;
    mx *= norm;
    my *= norm;
    mz *= norm;

    // Reference direction of Earth's magnetic field
    _2q1mx = 2.0f * q1 * mx;
    _2q1my = 2.0f * q1 * my;
    _2q1mz = 2.0f * q1 * mz;
    _2q2mx = 2.0f * q2 * mx;
    hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
    hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
    _2bx = sqrtf(hx * hx + hy * hy);
    _2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
    _4bx = 2.0f * _2bx;
    _4bz = 2.0f * _2bz;

    // Gradient decent algorithm corrective step
    s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax) + _2q2 * (2.0f * q1q2 + _2q3q4 - ay) - _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay) + (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    norm = sqrtf(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);    // normalise step magnitude
    norm = 1.0f/norm;
    s1 *= norm;
    s2 *= norm;
    s3 *= norm;
    s4 *= norm;

    // Compute rate of change of quaternion
    qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz) - beta * s1;
    qDot2 = 0.5f * (q1 * gx + q3 * gz - q4 * gy) - beta * s2;
    qDot3 = 0.5f * (q1 * gy - q2 * gz + q4 * gx) - beta * s3;
    qDot4 = 0.5f * (q1 * gz + q2 * gy - q3 * gx) - beta * s4;

    // Integrate to yield quaternion
    q1 += qDot1 * deltat;
    q2 += qDot2 * deltat;
    q3 += qDot3 * deltat;
    q4 += qDot4 * deltat;
    norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
    norm = 1.0f/norm;
    q[0] = q1 * norm;
    q[1] = q2 * norm;
    q[2] = q3 * norm;
    q[3] = q4 * norm;

}


//#define LOCATION_DEBUG_REGULATOR

// Similar to Madgwick scheme but uses proportional and integral filtering on the error between estimated reference vectors and
// measured ones. Source: https://github.com/kriswiner/MPU9250/blob/master/quaternionFilters.ino 
void Location::MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz)
{
    float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];   // short name local variable for readability
    float norm;
    float hx, hy, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez;
    float pa, pb, pc;

    // Auxiliary variables to avoid repeated arithmetic
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q1q4 = q1 * q4;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q2q4 = q2 * q4;
    float q3q3 = q3 * q3;
    float q3q4 = q3 * q4;
    float q4q4 = q4 * q4;   

    // Normalise accelerometer measurement
    norm = sqrtf(ax * ax + ay * ay + az * az);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f / norm;        // use reciprocal for division
    ax *= norm;
    ay *= norm;
    az *= norm;

    // Normalise magnetometer measurement
    norm = sqrtf(mx * mx + my * my + mz * mz);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f / norm;        // use reciprocal for division
    mx *= norm;
    my *= norm;
    mz *= norm;

    // Reference direction of Earth's magnetic field
    hx = 2.0f * mx * (0.5f - q3q3 - q4q4) + 2.0f * my * (q2q3 - q1q4) + 2.0f * mz * (q2q4 + q1q3);
    hy = 2.0f * mx * (q2q3 + q1q4) + 2.0f * my * (0.5f - q2q2 - q4q4) + 2.0f * mz * (q3q4 - q1q2);
    bx = sqrtf((hx * hx) + (hy * hy));
    bz = 2.0f * mx * (q2q4 - q1q3) + 2.0f * my * (q3q4 + q1q2) + 2.0f * mz * (0.5f - q2q2 - q3q3);

    // Estimated direction of gravity and magnetic field
    vx = 2.0f * (q2q4 - q1q3);
    vy = 2.0f * (q1q2 + q3q4);
    vz = q1q1 - q2q2 - q3q3 + q4q4;
    wx = 2.0f * bx * (0.5f - q3q3 - q4q4) + 2.0f * bz * (q2q4 - q1q3);
    wy = 2.0f * bx * (q2q3 - q1q4) + 2.0f * bz * (q1q2 + q3q4);
    wz = 2.0f * bx * (q1q3 + q2q4) + 2.0f * bz * (0.5f - q2q2 - q3q3);  

    // Error is cross product between estimated direction and measured direction of gravity
    ex = (ay * vz - az * vy) + (my * wz - mz * wy);
    ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
    ez = (ax * vy - ay * vx) + (mx * wy - my * wx);
    if (Ki > 0.0f)
    {
        eInt[0] += ex;      // accumulate integral error
        eInt[1] += ey;
        eInt[2] += ez;
        #ifdef LOCATION_DEBUG_REGULATOR
        Serial.println("REGLER----------");
        Serial.println(eInt[0]);
        Serial.println(eInt[1]);
        Serial.println(eInt[2]);
        #endif
    }
    else
    {
        eInt[0] = 0.0f;     // prevent integral wind up
        eInt[1] = 0.0f;
        eInt[2] = 0.0f;
    }
    #ifdef LOCATION_DEBUG_REGULATOR
    Serial.println(getHeading());
    #endif

  #define I_VALUE_MIN -10.0f
  #define I_VALUE_MAX 10.0f
    // NEW:  I - Value Limiter
    if(eInt[0] > I_VALUE_MAX ) {
      eInt[0] = I_VALUE_MAX;
    }
    if(eInt[0] < I_VALUE_MIN ) {
      eInt[0] = I_VALUE_MAX;
    }
    if(eInt[1] > I_VALUE_MAX ) {
      eInt[1] = I_VALUE_MAX;
    }
    if(eInt[1] < I_VALUE_MIN ) {
      eInt[1] = I_VALUE_MAX;
    }
    if(eInt[2] > I_VALUE_MAX ) {
      eInt[2] = I_VALUE_MAX;
    }
    if(eInt[2] < I_VALUE_MIN ) {
      eInt[2] = I_VALUE_MAX;
    }

    // Apply feedback terms
    gx = gx + Kp * ex + Ki * eInt[0];
    gy = gy + Kp * ey + Ki * eInt[1];
    gz = gz + Kp * ez + Ki * eInt[2];

    // Integrate rate of change of quaternion
    pa = q2;
    pb = q3;
    pc = q4;
    q1 = q1 + (-q2 * gx - q3 * gy - q4 * gz) * (0.5f * deltat);
    q2 = pa + (q1 * gx + pb * gz - pc * gy) * (0.5f * deltat);
    q3 = pb + (q1 * gy - pa * gz + pc * gx) * (0.5f * deltat);
    q4 = pc + (q1 * gz + pa * gy - pb * gx) * (0.5f * deltat);

    // Normalise quaternion
    norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
    norm = 1.0f / norm;
    q[0] = q1 * norm;
    q[1] = q2 * norm;
    q[2] = q3 * norm;
    q[3] = q4 * norm;

}
/**
 * This Function shows Herbie as Ascii art. It may be deleted, but I'd be sad if that would be done.
 */
void Location::aLittleJoke(){
Serial.println("###%##%%%%%%%%###((#%#%###%%%%%%%%%%%&&&%%%%%&&&%%%%##%%##%%%%%#######%%%%########%#%%%%%%&&%%%%%%%%%%%%%#%%%%%%%%%%%&&%%&&&&&&&&&&&&%%%%%%###%####(**//((((///"); 
Serial.println("((####%%%%%%%%#####((((##%%#########%%%&&&&&&%%%%%%%####%#%%%%%%%%%%%###%##############%%%%%%%%%%%####%%%%%%%%%&&%%%&&&&&&&&&&&&&&&&&%#%%&%#%%##((/**/(((#/*/()"); 
Serial.println("###########%%%##(###%%%###%%#####((((###%%%%###%&&%%%%##%%#%######%%%%####%%############%%%#%%%%%%%%%%%%%%%%%%%%%%%%%&&&&&&&&@&&@&&%((%%%#########(/((((//(/((#"); 
Serial.println("#######(((##(((((###(((##############((/(########%%%%%%%#######%%%%#%%%%####%%%%%######%%%%%%%%&&&&%%%&&&&&&&&&&&&&&&&&&&&&&@@@@@&%%&%%%######((#(/(/(///(##/%#"); 
Serial.println("###(#%%#((((####%%%#############%####%%%%%%#######%%%%%#######%%%%###%%%%%%%%%%%#######%%%%%&&&&&&&&&&&@&&&&&&&&&&&&&&&&&&@@@@@@@@#%%##########/((//(///(#####("); 
Serial.println("###%%%##((((#####%%#####%%%%%%%%%%###%%%%%%%%%%%##((((###%########%###%%##%##%%&&%%%%#%%&&%%%&&&&&&&&&&&&&&&&&&&&&&&&&&&&&@@@@@@%(%%####((#%#(((/##(////(##%%#("); 
Serial.println("#(%&&%#(((((##%####%###%%%%%%/((((/###%%%%%%%&%%%%%%#((#(##((#%%%%%%%%%####((#((((///**,,,,,,*/(##((##%%&&&&&&&&&&&&&&@@@@@@@@%#((##(#((((#(/((/(////**/((((#(#"); 
Serial.println("#####%#######(%%%%%%##%###%%#.,,*,,#%##%%%&%&&&&&&%&&&%(/**,.....,,,,,,,,..,,,   ,,,,,,,,,,,,,,,,,,,,,,.#&%%&@@&&&%&%%%%%%##(***/(((((((#(((((/////////(((((((("); 
Serial.println("%%####((###%#*#%%%%%%####%%#(.,*,,,#%%%###%%&%#(/*,...,,,,......,,,,,,,,,,,***   ,*****,,,,,,,,,,,,,,,,.,###((((((##((((((/***///((((((((//((//*///((((((/(((//"); 
Serial.println("*,,,**///**#&%&%%#(((////////***//*****,,,,,,,,,,,,,,,,,,,,******////(((##%%&&%##%%####(((((((####%##(/,,,/(##((((((((((#########((((((///(/*///(((((((((((((##"); 
Serial.println("*/(##/*******/**,*,*****,,,/%%(*%##/*,,,,,,,,,****//(#%%%##(/*,..        .**.  ,****,                .,***//(((((((##############(////(((#((((((/(((((/(((((("); 
Serial.println("*(#%%(/*****************////#&(*&&%%(//*,,**(##/*.                         .(/*  ,/////                     .,,,,/////////////////((((((((((((////****((((((((("); 
Serial.println("#%%&%%#(/******//(###/((#((/*(/*&@(****//,                  ..,*//(((##((//////**///(((######(##(////(((/,      ..***,,,,,,,,,***///////((((((((/(//((((((((/(("); 
Serial.println("%&&&&&///////###%%%%%%%%##((###(*,,.           .*/(((//*//(##%&&&&&&&%%%#######(((((((((((((((((####%%//*     ***********,,,,,,,,,***//(/((//(((((/////(((("); 
Serial.println("**************/((((((////((((%%&/..       *(///#%&&%%#####(###(((///***********,*(###################((((((#%,, ..,****************,,,,,,,,,,,**/////////////(("); 
Serial.println("/**//****//////*/////*/////#%&@#,      *//%&#(((((((((###(/************,***,*,*#####################(#####(%,/.,,*********************,,,,,,,,,,,,,***////(//"); 
Serial.println("***********************/*/(((#(/,    ,*&%#((((((((((((((((####(****//((((#(((#/*/#######(##################((/#**.,,*****************************,,,,,,,,,,,***"); 
Serial.println("************,,,,,,,,,***,,,,,,,/,   ,/##((((########((((((###(***(##########/,,,/#####(###################(//#,,,,*************************************,,,,,,"); 
Serial.println("/***********,,,,,,,,,,,*,,,**,*/,.  /%##(#/###((###((######(####/,,*******,,,,,,,,/##########(((((##(((##(###(/((/.,,******************************************"); 
Serial.println(",,,,,,,,,,,,,,,,,,*,,,,/,,,,,**/,. **%##(#/###((((((((#####(####(,,,,,,*,,,,,,,,,,(########((((((((#((((((##(((/%,*.,,*****************************************"); 
Serial.println(",,,,,,,,,,,,,,,,,,,,,,,****,**//,../(##((#*###((((((((###########//((####################(((((((##((((////(#((///(*,,,*****************************************"); 
Serial.println(",,,,,*******,,,,,,,,,,,,*....*/*, **%(##((/(##((((((#(####(######(((///////////////(####(((((((##((((///***/(((//%,/.,,****************************************"); 
Serial.println(",,,,,,,,,,,,..,,,,,,..,,/,,.,*/*. /*%###((#(##(((((((((########((((######((((//////((((((*,,**//((######(**,/((((#(,,,,,*******,*%%(***************************"); 
Serial.println(".......,,,,,,,,,.,.,,,,,*,,,,*/*. /*%###(##(((///(((((((##(########%&&&&&%%####(//*********///*****/((##(/****((##**.,,,*****(##(*(#(**************************"); 
Serial.println("////***,,,,,.,,,,,*,,,,,,*,,,**,. /*#(((((((((#%%#%%%%%##(/****,,,,........,. *(/*  ,/////,.*,.            ...........,,,****/*/#(#((**************************"); 
Serial.println("(((((((((/**,,,,,,,,,,,,,*,,,*,,../*%(/***(###/*//**,,..  ...,,...............,///   /////* ...................... ....,,*****/##((/***************************"); 
Serial.println("/////**,.,,,,,,,,,,,,,,,,*,,..,,...**////****,................ .        .. ,/  *//.  */*///.............................,***************/**********************"); 
Serial.println(",,....,,,,,*,,,,,,,,,,,,,**,/**,.....     ................................ ./..*//*   /////*.........................,,****((*,,****************,,,************"); 
Serial.println("*,,,,,,,,,,,,,,,,,,,,,,,,**,**,...  ......................................  ....*//.  *****/.                   . ..,,**/**/(/**************,,,,***,.,*********"); 
Serial.println(",,*,,,,,,,,,,***,,,,,,,,,,/***,... ..........................                   ,**.  .*****,                       ,,****(((/******************,,,,***,.,*****"); 
Serial.println(",,,,,,,,,,,,,,,,,,,,,,,,***/**,,,,,..............                               .***   ******.                     .,****/////**********************,,,***,,,,*"); 
Serial.println(",,,,,,,,,,,,,,,,,,,****,/*///*,**,,,..             .                        .....*//.  *******                  . ,****,/////***************************,,,***,"); 
Serial.println(",,,,,,,,,,,,,,,**,,********//(/*,**,,.                    .          ,,,.            .,*////**.               ....,***.*//*///******************************,,,"); 
Serial.println(",,,,,,,,,,,,,,,,,,,,*******//**/(,,*,,.                         .,,       ..   .,/(###(/*../(/, . ........    ...,*****/***///*********************************"); 
Serial.println(",,,,,,,,,,,,,,,,,,,*******,*/**,,(,,,,,.               ...    ,, (######((//      .   .(###, ,(   ..............,***,,,,*,*      .....,************************"); 
Serial.println(",,,,,,,,,,,,,,,,**********,*#(**,,/*.,*,.  ...      ....... .*   ##(#.                .((##*  .(.......... ....,,**,,..**,*, .............,********************"); 
Serial.println(",,,,,,,,,,,,,,************,#&%****,,*..,,... ...............*   .########(/,       /######/    /,.............,,**,,.../(((/,...............*******************"); 
Serial.println(",,,,,,,,,,,***************,,(*... .......,................./.   .%###(((#####,      . ./###(   /,............,,,**,...      ................,******************"); 
Serial.println(",,,,,,,,*************,,.         .****  .,,................*.            .####         ./###,  (............,,,**,,...  .....................,*****************"); 
Serial.println(",,,,,,,,*,*********,.            *////* ..,*,.............../           .(###/    (########/  ((...........,,,***,... ...      ....,,,....,,.,*****************"); 
Serial.println(",,,,,,,**********,               .*//,. ...,,,,............../*   .##%%%%%%#,    ,/#%%%#(,  /#((.........,,,****,,. . .*****,****...,,,,,,,,,,*****************"); 
Serial.println(",,**************,                        ...,,,................(. ,(#%%#/,                *#((((,.......,,,****,,,., *,*/*****///**,*,**,,,,,,*****************"); 
Serial.println("***************,.                ..........,,,,*,................*/*.                 ,//.((((((,......,,,***/*,,,,.*//*****//////((*/,***,,,******************"); 
Serial.println(",,*************,       ..     ...,*****,. ...,,,*,,...................,*///////,,(/(##(.  /(((((/.,...,,,***//,*,,.*((////////((/(###//************************"); 
Serial.println(",,*************. .......... . ,*,*/////*,,,.,,,,,*,,...........................  ,,*###.  *((((((,,..,,,****/*,,,*.*(*//(/,.. ./((###/(************************"); 
Serial.println("***************..........  ..*///****,**////,*,,,,**,,,...............,,,,,,,,.,,,,,(##*  ,((((((,..,,,****//,,,,**,#/,*/*     *((#%#((*//*******///////////***"); 
Serial.println("***************.............*///***////((//((,/,,,,/*,,,,,,,,,,,,,,,,,,,,,,,,,,,.,,,(##/  ,##((##*.,,*****//*,,,,,(//#(***(((##(####((/*********////////////***"); 
Serial.println("***************...........,./(*////////////(#*/*,,,,/*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,(##(. .##(###/,,*****///,,,,,,,/#/#####(((######(/*/*********//////////////"); 
Serial.println("***************,........,,*,((*//(*,.  .*/*(#*(/,,,,*/**,,,,,,,,,,,,,,,,,,,,,,,,,,,,/###.  ((##(((,*****///**,*,  .,*(#(####((###(((************///////////////"); 
Serial.println("***************,........,,**(#/*//,   ,/(//(#*(*,,,,,,/**,,,,,,,,,,,,,,,,,,,,,,*,.***###*  ((((((#*****///**,,/(  ,(,,**(##%%%%#(*,.   .,******////////////////"); 
Serial.println("*******//******,.......,,,,(/###(//(((((((##/#/,,.,,,,****,,,,,,,,,,,,,,,,,,,,,*, /**###/  /##((##***////****,/#**(#,...   ..,*/((####((###/***////((//////////"); 
Serial.println("*********,,,*//*,,,....,,,,,//(########%##(##/,   //,,,*****,,,,,,,,,,,,,,,,,,,,,./**###(  ,((/***.        ..,(#(####%##(///******///****/##/*///((/((((///////"); 
Serial.println("******,,,****//*,,,,,..,,,,***(#((((##(##%#/*,*. *((,,,***/**,,,,,,......*/(((##%%&%%&&&&%%&@@@######(///(#%(#####(*************////////(#/**(%#((((/////////"); 
Serial.println("*********///////,,,..,,,,.     ..        ..  ..(((#(,......,,***////%@@@@&%&&&&&&&&&&&&&&&&&&&&%%%#((#(*/##%(*/######*************//***,.,(#(/(#((//(((((/((("); 
Serial.println("*********///////*,,,,,,,/#%(//(((((((((((((/(#######(#*,,*/////***(&@@@@@&@&&&&&&&&%%&%%&%%%&%%&%##((/(&&%(/(##(###*,,,...    ..,,*/(########(%#(((((((/((((/"); 
Serial.println("**///////////////*,,,,,(#(****//////**********/#((##(*,(&&&///(((///(&&%%%%#%%&%&%&&&%&&%&&&&%&%#(/**,,..    ,/((####*...,,,*(###############(((%#(((((((//////"); 
Serial.println("////////////////(%*,,,*(%/*//////////****/*****#///##(/((#(////(((//(%&%&%%%%&&%%&&&&%&&&&%&%%&%*             .*(((//#########((((((((((###%%%&&%(/////////////"); 
Serial.println("/////////////////%%(*../%%(///////////////***,,#(/,##%(*,....       ,%&%&%%%&&&&&&&&&&%&&%%&&&&(((###(###%%%##%(**(###########%%&&&&@@@@@@@@&&%(((((((((/((//"); 
Serial.println("/////////////////#&%#*.*(         .     .,,,.,,/((/(##/,..**,,. ...,/%&&&%##%%###################((((######%%%%%&%#(##%%%&&&&@@@@@@@@@@@@@@@@&&&%(((((((((((///"); 
Serial.println("/////////////////(%%%%%((***,,,,,,,,..,,**/////(* ,###(///((#((###############((#########%%%%%%%%%%%%%######&%#%&&&&@@@@@@&&&&@@@@@@@@@@@@@@&&&&%(((((((((((((("); 
Serial.println("/////////////////(%%#((((((((((((((((#########(((#%%%%%%%%%%%%%%&%%&&&&&&&&&&&&&%%%%%%%%######%%%%&&&@@@@@@@@@@@@@@@@@@@@&&&@@@@@@@@@@@@@&&&&&%(((((((((((((("); 
Serial.println("//////////////////%%%&&&&@@@@@@@@@@@@&&&&&&&&&%%%##%&&%%%%%%%%&&&&&&%&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&@@@@@@@@@@@@@&&@@(((((((((((((("); 
Serial.println("/////////(///((///#%%&&&&&&&&&@@@@@@@&%&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&((((((((((((((("); 
Serial.println("///////////((/((//(%%&&&&&&&&&&@@@@@@&%&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@((((((((((((((/("); 

}