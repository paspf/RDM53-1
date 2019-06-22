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

  int i;
  for(i=0;i<20;i++){
    obstBuffer.obstacles[i].obstacleDetectionTime = millis();
    obstBuffer.obstacles[i].obstacleID = i;

    if(lidarSensors.measureLidar1.RangeMilliMeter <= 1200 && lidarSensors.measureLidar4.RangeMilliMeter <= 1200) {
    obstBuffer.obstacles[i].distanceObstacteToCar = lidarSensors.measureLidar1.RangeMilliMeter;
    obstBuffer.obstacles[i].angleObstacleToCar = 0;
    }
    if(lidarSensors.measureLidar3.RangeMilliMeter <= 1200 && lidarSensors.measureLidar4.RangeMilliMeter <= 1200){
    obstBuffer.obstacles[i].distanceObstacteToCar = lidarSensors.measureLidar4.RangeMilliMeter;
    obstBuffer.obstacles[i].angleObstacleToCar = 45;
    }
    if(lidarSensors.measureLidar5.RangeMilliMeter <= 1200){
    obstBuffer.obstacles[i].distanceObstacteToCar = lidarSensors.measureLidar5.RangeMilliMeter;
    obstBuffer.obstacles[i].angleObstacleToCar = 90;
    }
    if(lidarSensors.measureLidar6.RangeMilliMeter <= 1200){
    obstBuffer.obstacles[i].distanceObstacteToCar = lidarSensors.measureLidar6.RangeMilliMeter;
    obstBuffer.obstacles[i].angleObstacleToCar = 180;
    }
    if(lidarSensors.measureLidar7.RangeMilliMeter <= 1200){
    obstBuffer.obstacles[i].distanceObstacteToCar = lidarSensors.measureLidar7.RangeMilliMeter;
    obstBuffer.obstacles[i].angleObstacleToCar = 270;
    }
    if(lidarSensors.measureLidar1.RangeMilliMeter <= 1200 && lidarSensors.measureLidar2.RangeMilliMeter <= 1200){
    obstBuffer.obstacles[i].distanceObstacteToCar = lidarSensors.measureLidar1.RangeMilliMeter;
    obstBuffer.obstacles[i].angleObstacleToCar = 315;
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
int ObstacleInterface::getstartposition(){

}

