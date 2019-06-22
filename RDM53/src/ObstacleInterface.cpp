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

//ermittelt die Winkelposition des Hindernisses relativ zum Fahrzeug
int ObstacleInterface::initobstacleposition(){

  if(lidarSensors.measureLidar1.RangeStatus != 4 && lidarSensors.measureLidar4.RangeStatus != 4) {
    angleofobstacle = 0;
  }
  else if(lidarSensors.measureLidar3.RangeStatus != 4 && lidarSensors.measureLidar4.RangeStatus != 4){
    angleofobstacle = 45;
  }
  else if(lidarSensors.measureLidar5.RangeStatus != 4){
    angleofobstacle = 90;
  }
  else if(lidarSensors.measureLidar6.RangeStatus != 4){
    angleofobstacle = 180;
  }
  else if(lidarSensors.measureLidar7.RangeStatus != 4){
    angleofobstacle = 270;
  }
  else if(lidarSensors.measureLidar1.RangeStatus != 4 && lidarSensors.measureLidar2.RangeStatus != 4){
    angleofobstacle = 315;
  }
  else {
    Serial.print("No Obstacle");
    return -1;
  }
  return angleofobstacle;
}


//ermittelt den Abstand Lidarsensor(Fahrzeug) zum Hindernis
int ObstacleInterface::initobstacledistance(){

  obstBuffer.obstacles[0].obstacleDetectionTime = millis();
  switch (angleofobstacle) {
    case 0: distanceObstacteToCar = lidarSensors.measureLidar1.RangeMilliMeter; break;
    case 45: distanceObstacteToCar = lidarSensors.measureLidar4.RangeMilliMeter; break;
    case 90: distanceObstacteToCar = lidarSensors.measureLidar5.RangeMilliMeter; break;
    case 180: distanceObstacteToCar = lidarSensors.measureLidar6.RangeMilliMeter; break;
    case 270: distanceObstacteToCar = lidarSensors.measureLidar7.RangeMilliMeter; break;
    case 315: distanceObstacteToCar = lidarSensors.measureLidar1.RangeMilliMeter; break;
  }
  
  return distanceObstacteToCar;
}

void ObstacleInterface::setobstacleID(int oID){

    obstacleID = oID;
}

int ObstacleInterface::getstartposition(){

}

