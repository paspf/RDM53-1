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
extern  lidar lidarSensors; //lidar Objekt erstellt um mit Sensordaten arbeiten zu können
extern obstBuff obstBuffer;

//ermittelt die Hindernissdaten zum Fahrzeug
void ObstacleInterface::initobstacledata(){

  int angleObstacleToCar;
  int distanceObstacleToCar;
  int i;
  for(i=0;i<20;i++){

    if(lidarSensors.measureLidar1.RangeMilliMeter <= 1200 && lidarSensors.measureLidar4.RangeMilliMeter <= 1200) {
    distanceObstacleToCar = lidarSensors.measureLidar1.RangeMilliMeter;
    angleObstacleToCar = 0;
    fillObstacleStructure(i,angleObstacleToCar,distanceObstacleToCar);
    continue;
    }
    if(lidarSensors.measureLidar3.RangeMilliMeter <= 1200 && lidarSensors.measureLidar4.RangeMilliMeter <= 1200){
    distanceObstacleToCar = lidarSensors.measureLidar4.RangeMilliMeter;
    angleObstacleToCar = 45;
    fillObstacleStructure(i,angleObstacleToCar,distanceObstacleToCar);
    continue;
    }
    if(lidarSensors.measureLidar5.RangeMilliMeter <= 1200){
    distanceObstacleToCar = lidarSensors.measureLidar5.RangeMilliMeter;
    angleObstacleToCar = 90;
    fillObstacleStructure(i,angleObstacleToCar,distanceObstacleToCar);
    continue;
    }
    if(lidarSensors.measureLidar6.RangeMilliMeter <= 1200){
    distanceObstacleToCar = lidarSensors.measureLidar6.RangeMilliMeter;
    angleObstacleToCar = 180;
    fillObstacleStructure(i,angleObstacleToCar,distanceObstacleToCar);
    continue;
    }
    if(lidarSensors.measureLidar7.RangeMilliMeter <= 1200){
    distanceObstacleToCar = lidarSensors.measureLidar7.RangeMilliMeter;
    angleObstacleToCar = 270;
    fillObstacleStructure(i,angleObstacleToCar,distanceObstacleToCar);
    continue;
    }
    if(lidarSensors.measureLidar1.RangeMilliMeter <= 1200 && lidarSensors.measureLidar2.RangeMilliMeter <= 1200){
    distanceObstacleToCar = lidarSensors.measureLidar1.RangeMilliMeter;
    angleObstacleToCar = 315;
    fillObstacleStructure(i,angleObstacleToCar,distanceObstacleToCar);
    continue;
    }
    else {
    Serial.print("No Obstacle in proximity");
    }   
  }
}

/*void ObstacleInterface::setobstacleID(int oID){

    obstacleID = oID;
}
 */
int ObstacleInterface::getstartPosition(){

}

int ObstacleInterface::checkIfObstacleExist(int angl){

  for(int i=0;i<20;i++){
    if(obstBuffer.obstacles[i].angleObstacleToCar == angl){
      return 0;
    }
  }
  return 1;
}

void ObstacleInterface::fillObstacleStructure(int i,int angle, int distance){

    if (!checkIfObstacleExist(angle)){
    obstBuffer.obstacles[i].angleObstacleToCar = angle;
    obstBuffer.obstacles[i].distanceObstacteToCar = distance;
    obstBuffer.obstacles[i].obstacleDetectionTime = millis();
    obstBuffer.obstacles[i].obstacleID = i;
    }

}

