/*
 * This file contains the informations for 
 * the obstacle interface 
 * 
 * 
 * Date: 2019 06 20
 * Author: Taha Tekdemir
 */

#include <ObstacleInterface.h>
#include <lidar.h>
#include <HCSR04P.h>
#include "connectivity.h"

extern  lidar lidarSensors; //lidar object to work directly with the sensor data (RangeMillimeter)
extern obstBuff obstBuffer;

extern HCSR04P ultraSonic;

//detect the obstacledata to the car 
void ObstacleInterface::initobstacledata(){

  int angleObstacleToCar;
  int distanceObstacleToCar;
  int i;
  for(i=0; i<20; i++){ //20 buffer obstacles (virtual)

    //obstacle in front of the car (directly)
    if(ultraSonic.dist() <= 1200 && lidarSensors.measureLidar[0].RangeMilliMeter <= 1200 && lidarSensors.measureLidar[3].RangeMilliMeter <= 1200) {
      distanceObstacleToCar = lidarSensors.measureLidar[0].RangeMilliMeter;
      angleObstacleToCar = 0;
      fillObstacleStructure(i,&angleObstacleToCar,&distanceObstacleToCar);
      continue;
    }
    //obstacle at the right front of the car 
    if(lidarSensors.measureLidar[2].RangeMilliMeter <= 1200 && lidarSensors.measureLidar[3].RangeMilliMeter <= 1200){
      distanceObstacleToCar = lidarSensors.measureLidar[3].RangeMilliMeter;
      angleObstacleToCar = 45;
      fillObstacleStructure(i,&angleObstacleToCar,&distanceObstacleToCar);
      continue;
    }
    //obstacle at the rigth side of the car 
    if(lidarSensors.measureLidar[4].RangeMilliMeter <= 1200){
      distanceObstacleToCar = lidarSensors.measureLidar[4].RangeMilliMeter;
      angleObstacleToCar = 90;
      fillObstacleStructure(i,&angleObstacleToCar,&distanceObstacleToCar);
      continue;
    }
    //obstacle behind the car 
    if(lidarSensors.measureLidar[5].RangeMilliMeter <= 1200){
      distanceObstacleToCar = lidarSensors.measureLidar[5].RangeMilliMeter;
      angleObstacleToCar = 180;
      fillObstacleStructure(i,&angleObstacleToCar,&distanceObstacleToCar);
      continue;
    }
    //obstacle at the left side of the car 
    if(lidarSensors.measureLidar[6].RangeMilliMeter <= 1200){
      distanceObstacleToCar = lidarSensors.measureLidar[6].RangeMilliMeter;
      angleObstacleToCar = 270;
      fillObstacleStructure(i,&angleObstacleToCar,&distanceObstacleToCar);
      continue;
    }
    //obstacle at the left front of the car 
    if(lidarSensors.measureLidar[0].RangeMilliMeter <= 1200 && lidarSensors.measureLidar[1].RangeMilliMeter <= 1200){
      distanceObstacleToCar = lidarSensors.measureLidar[0].RangeMilliMeter;
      angleObstacleToCar = 315;
      fillObstacleStructure(i,&angleObstacleToCar,&distanceObstacleToCar);
      continue;
    }
    else {
      sendStringln("No Obstacle in proximity");
    }   
  }
}

/*void ObstacleInterface::setobstacleID(int oID){

    obstacleID = oID;
}
 */
int ObstacleInterface::getstartPosition(){
  return 0;
}

int ObstacleInterface::checkIfObstacleExist(int *a){

  for(int i=0;i<20;i++){
    if(obstBuffer.obstacles[i].angleObstacleToCar == *a){
      return 0;
    }
  }
  return 1;
}

void ObstacleInterface::fillObstacleStructure(int i,int *angle, int *distance){

    if (!checkIfObstacleExist(angle)){
      obstBuffer.obstacles[i].angleObstacleToCar = *angle;
      obstBuffer.obstacles[i].distanceObstacteToCar = *distance;
      obstBuffer.obstacles[i].obstacleDetectionTime = millis();
      obstBuffer.obstacles[i].obstacleID = i;
    }

}

