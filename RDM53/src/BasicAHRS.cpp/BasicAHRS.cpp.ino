/*
  Advanced_I2C.ino
  Brian R Taylor
  brian.taylor@bolderflight.com

  Copyright (c) 2017 Bolder Flight Systems

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
  and associated documentation files (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge, publish, distribute,
  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or
  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MPU9250.h"
#include <MadgwickAHRS.h>
#include <math.h>

Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);
int status;
bool calibrate = false;

void setup() {
  // serial to display data
  Serial.begin(115200);
  while (!Serial) {}

  // start communication with IMU
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {}
  }
  // setting the accelerometer full scale range to +/-8G
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  // setting the gyroscope full scale range to +/-500 deg/s
  IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  // setting DLPF bandwidth to 20 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  // setting SRD to 19 for a 50 Hz update rate
  IMU.setSrd(19);


  //Setting Calibration Values
  IMU.setMagCalX(13.9, 0.98);
  IMU.setMagCalY(5.8, 1.66);
  IMU.setMagCalZ(5.71, 0.72);

  //Calibration
  if (calibrate == true) {
    if (IMU.calibrateGyro() > 0)
    {
      Serial.println("Gyro Calibration Successful");
    }
    else
    {
      Serial.println("Gyro Calibration failed");
    }

    if (IMU.calibrateAccel() > 0)
    {
      Serial.println("Accelerometer Calibration Successful");
    }
    else
    {
      Serial.println("Accelerometer Calibration failed");
    }

    Serial.println("Slowly move sensor in figure 8");
    if (IMU.calibrateMag() > 0)
    {
      Serial.println("Magnetometer Calibration Successful");
    }
    else
    {
      Serial.println("Magnetometer Calibration failed");
    }
    Serial.print("MagBiasX");
    Serial.println(IMU.getMagBiasX_uT());

    Serial.print("MagBiasY");
    Serial.println(IMU.getMagBiasY_uT());

    Serial.print("MagBiasZ");
    Serial.println(IMU.getMagBiasZ_uT());

    Serial.print("MagScaleX");
    Serial.println(IMU.getMagScaleFactorX());

    Serial.print("MagScaleY");
    Serial.println(IMU.getMagScaleFactorY());

    Serial.print("MagScaleZ");
    Serial.println(IMU.getMagScaleFactorZ());
  }

  // initialize variables to pace updates to correct rate
  microsPerReading = 1000000 / 25;
  microsPrevious = micros();
}

void loop() {
  // read the sensor
  int aix, aiy, aiz;
  int gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;
  unsigned long microsNow;

  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {
    IMU.readSensor();
    
    float gx = IMU.getGyroX_rads() * 180/PI;
    float gy = IMU.getGyroY_rads() * 180/PI;
    float gz = IMU.getGyroZ_rads() * 180/PI;
    float ax = IMU.getAccelX_mss();
    float ay = IMU.getAccelY_mss();
    float az = IMU.getAccelZ_mss();
    float mx = IMU.getMagX_uT();
    float my = IMU.getMagY_uT();
    float mz = IMU.getMagZ_uT();
    filter.update( gx,  gy,  gz,  ax,  ay,  az,  mx,  my,  mz);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);


  }

  // display the data
  /*
    Serial.print(IMU.getAccelX_mss(),6);
    Serial.print("\t");
    Serial.print(IMU.getAccelY_mss(),6);
    Serial.print("\t");
    Serial.print(IMU.getAccelZ_mss(),6);
    Serial.print("\t");
    Serial.print(IMU.getGyroX_rads(),6);
    Serial.print("\t");
    Serial.print(IMU.getGyroY_rads(),6);
    Serial.print("\t");
    Serial.print(IMU.getGyroZ_rads(),6);
    Serial.print("\t");
    Serial.print(IMU.getMagX_uT(), 6);
    Serial.print(" ");
    Serial.print(IMU.getMagY_uT(), 6);
    Serial.print(" ");
    Serial.print(IMU.getMagZ_uT(), 6);
    Serial.print(" ");
  
  //Serial.println(IMU.getTemperature_C(),6);
  delay(20);
  */
}
