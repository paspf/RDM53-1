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
extern  lidar ldsensor; //lidar Objekt erstellt um mit Sensordaten arbeiten zu können


//ermittelt die Winkelposition des Hindernisses relativ zum Fahrzeug
int ObstacleInterface::initobstacleposition(){

  if(ldsensor.measureLidar1.RangeStatus != 4 && ldsensor.measureLidar4.RangeStatus != 4) {
    angleofobstacle = 0;
  }
  else if(ldsensor.measureLidar3.RangeStatus != 4 && ldsensor.measureLidar4.RangeStatus != 4){
    angleofobstacle = 45;
  }
  else if(ldsensor.measureLidar5.RangeStatus != 4){
    angleofobstacle = 90;
  }
  else if(ldsensor.measureLidar6.RangeStatus != 4){
    angleofobstacle = 180;
  }
  else if(ldsensor.measureLidar7.RangeStatus != 4){
    angleofobstacle = 270;
  }
  else if(ldsensor.measureLidar1.RangeStatus != 4 && ldsensor.measureLidar2.RangeStatus != 4){
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

  starttime = millis();
  switch (angleofobstacle){
    case 0: distancecar = ldsensor.measureLidar1.RangeMilliMeter; break;
    case 45: distancecar = ldsensor.measureLidar4.RangeMilliMeter; break;
    case 90: distancecar = ldsensor.measureLidar5.RangeMilliMeter; break;
    case 180: distancecar = ldsensor.measureLidar6.RangeMilliMeter; break;
    case 270: distancecar = ldsensor.measureLidar7.RangeMilliMeter; break;
    case 315: distancecar = ldsensor.measureLidar1.RangeMilliMeter; break;
  }
  
  return distancecar;
}

void ObstacleInterface::setobstacleID(int oID){

    obstacleID = oID;
}

int ObstacleInterface::getstartposition(){

}

