#include <AHRS.h>

int AHRSInterface::initAHRS(){
    int status = IMU.begin();
    if (status == 0){
        IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);
        IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
        IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);

        IMU.setMagCalX(13.9, 0.98);
        IMU.setMagCalY(5.8, 1.66);
        IMU.setMagCalZ(5.71, 0.72);
    }
    return status;
}

int AHRSInterface::setAHRSrange(uint AccRange, uint GyrRange, uint MagBandwidth){
    int error = 0x0;

    switch (AccRange)
    {
    case 0x0:
        error += 0x1 * IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);
        break;
    case 0x1:
        error += 0x1 * IMU.setAccelRange(MPU9250::ACCEL_RANGE_4G);
        break;
    case 0x2:
        error += 0x1 * IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
        break;
    case 0x3:
        error += 0x1 * IMU.setAccelRange(MPU9250::ACCEL_RANGE_16G);
        break;
    default:
        error += 0x1;
        break;
    }

    switch (GyrRange)
    {
    case 0x0:
        error += 0x2 * IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
        break;
    case 0x1:
        error += 0x2 * IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
        break;
    case 0x2:
        error += 0x2 * IMU.setGyroRange(MPU9250::GYRO_RANGE_1000DPS);
        break;
    case 0x3:
        error += 0x2 * IMU.setGyroRange(MPU9250::GYRO_RANGE_2000DPS);
        break;
    default:
        error += 0x2;
        break;
    }

    switch (MagBandwidth)
    {
    case 0x0:
        error += 0x4 * IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_5HZ);
        break;
    case 0x1:
        error += 0x4 * IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_10HZ);
        break;
    case 0x2:
        error += 0x4 * IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
        break;
    case 0x3:
        error += 0x4 * IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_41HZ);
        break;
    case 0x4:
        error += 0x4 * IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_92HZ);
        break;
    case 0x5:
        error += 0x4 * IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_184HZ);
        break;
    default:
        error += 0x4;
        break;
    }
    return error;
}
void AHRSInterface::setAccCal(float xBias, float xScale, float yBias, float yScale, float zBias, float zScale){
    IMU.setAccelCalX(xBias, xScale); 
    IMU.setAccelCalY(xBias, xScale); 
    IMU.setAccelCalZ(xBias, xScale); 
}
void AHRSInterface::setGyroCal(float xBias, float yBias, float zBias){
    IMU.setGyroBiasX_rads(xBias);
    IMU.setGyroBiasX_rads(yBias);
    IMU.setGyroBiasX_rads(zBias);
}
void AHRSInterface::setMagCal(float xBias, float xScale, float yBias, float yScale, float zBias, float zScale){
    IMU.setMagCalX(xBias, xScale);
    IMU.setMagCalX(yBias, yScale);
    IMU.setMagCalX(zBias, zScale);
}
int AHRSInterface::calAccGyro(){
    int status = 0x1 * IMU.calibrateAccel();
    status = 0x2 * IMU.calibrateGyro();
    return status;
}
int AHRSInterface::calFull(){
    sendString("Acceleration Calibration. Do not move RDM");
    int status = 0x1 * IMU.calibrateAccel();
    status = 0x2 * IMU.calibrateGyro();
    sendString("Magnetic Calibration. Move RDM in an 8 Pattern until complete.");
    status = 0x4 * IMU.calibrateMag();
    return status;
}
void AHRSInterface::updateFilter(){
    updateVal();
    filter.update(gx,  gy,  gz,  ax,  ay,  az,  mx,  my,  mz);
}
void AHRSInterface::updateVal(){
    float gx = IMU.getGyroX_rads() * 180/PI;
    float gy = IMU.getGyroY_rads() * 180/PI;
    float gz = IMU.getGyroZ_rads() * 180/PI;
    float ax = IMU.getAccelX_mss();
    float ay = IMU.getAccelY_mss();
    float az = IMU.getAccelZ_mss();
    float mx = IMU.getMagX_uT();
    float my = IMU.getMagY_uT();
    float mz = IMU.getMagZ_uT();
}

float AHRSInterface::getAccX(){return ax;}
float AHRSInterface::getAccY(){return ay;}
float AHRSInterface::getAccZ(){return az;}

float AHRSInterface::getGyrX(){return gx;}
float AHRSInterface::getGyrY(){return gy;}
float AHRSInterface::getGyrZ(){return gz;}

float AHRSInterface::getMagX(){return mx;}
float AHRSInterface::getMagY(){return my;}
float AHRSInterface::getMagZ(){return mz;}

float AHRSInterface::getHeading(){return heading;}
float AHRSInterface::getPitch(){return pitch;}
float AHRSInterface::getRoll(){return roll;}


    