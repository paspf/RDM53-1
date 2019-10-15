/*
 * This file contains the informations for 
 * the obstacle interface 
 * 
 * 
 * Creation date: 2019 06 20
 * Author: Taha Tekdemir
 * Co-author: Jan Kühnemund
 */

#include <ObstacleInterface.h>
#include <lidar.h>
#include <HCSR04P.h>
#include "connectivity.h"
#include "location.h"

extern lidar lidarSensors; //lidar object to work directly with the sensor data (RangeMillimeter)
//extern obstBuff obstBuffer;

extern HCSR04P ultraSonic;

Location myLocation;

ObstacleInterface::obstacle myObstacle = NULL;

//detect the obstacledata to the car 
void ObstacleInterface::initobstacledata(){

  int angleObstacleToCar;
  int distanceObstacleToCar;
  int i;
  for(i=0; i<20; i++){ //20 buffer obstacles (virtual)

    //obstacle in front of the car (directly)
    if(ultraSonic.getDist() <= 1200 && lidarSensors.measureLidar[0].RangeMilliMeter <= 1200 && lidarSensors.measureLidar[3].RangeMilliMeter <= 1200) {
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

ObstacleInterface::obstacle ObstacleInterface::checkIfObstacleExist(obstacle currentObstacle, float xTmp, float yTmp){
  while (currentObstacle != NULL){
    float xDiff = currentObstacle->xCoordinate - xTmp;
    if (xDiff < 10 && xDiff > -10){
        float yDiff = currentObstacle->yCoordinate - yTmp;
        if(yDiff < 10  && yDiff > -10){
          return currentObstacle;
        }
    }
    currentObstacle = currentObstacle->next;
  }
  return NULL;
}

void ObstacleInterface::updateObstacle(obstacle thisObstacle, float xTmp, float yTmp){
  float anKatQ = pow(xTmp - thisObstacle->xCoordinate, 2);
  float gegKatQ = pow(yTmp - thisObstacle->yCoordinate, 2);
  float hypQ = pow(thisObstacle->radius, 2);

  if(anKatQ + gegKatQ > hypQ){
    thisObstacle->radius = sqrtf(anKatQ + gegKatQ);
    thisObstacle->xCoordinate = (xTmp + thisObstacle->xCoordinate) / 2;
    thisObstacle->yCoordinate = (yTmp + thisObstacle->yCoordinate) / 2;
    thisObstacle->updateTime = millis();
  }
}

void ObstacleInterface::addObstacle(obstacle thisObstacle, float xTmp, float yTmp){
  obstacle newObstacle = (obstacle) malloc(sizeof(struct obstacle_struct));
  while (thisObstacle->next != NULL){
    thisObstacle = thisObstacle->next;
  }
  newObstacle->detectionTime = millis();
  newObstacle->xCoordinate = xTmp;
  newObstacle->yCoordinate = yTmp;
  newObstacle->next = thisObstacle->next; //NULL
  thisObstacle->next = newObstacle;
}

void ObstacleInterface::checkForObstacles(){
  //obstacle in front of the car (directly)
    if(ultraSonic.getDist() <= maxRange)
    {
      float xOffset = ultraSonicDistance * cos(myLocation.getHeading() + ultraSonicDeviation);
      float yOffset = ultraSonicDistance * sin(myLocation.getHeading() + ultraSonicDeviation);
      float xTmp = ultraSonic.getDist()*cos(myLocation.getHeading()+ultraSonicHeading)+myLocation.getPosX()+xOffset;
      float yTmp = ultraSonic.getDist()*sin(myLocation.getHeading()+ultraSonicHeading)+myLocation.getPosY()+yOffset;
      obstacle thisObstacle = checkIfObstacleExist(myObstacle, xTmp, yTmp);
      if(thisObstacle != NULL ){
        updateObstacle(thisObstacle, xTmp, yTmp);
      }
      else{
        addObstacle(myObstacle, xTmp, yTmp);
      }
    }
    if(lidarSensors.measureLidar[0].RangeMilliMeter <= maxRange){
      float xOffset = lidar0Distance * cos(myLocation.getHeading() + lidar0Deviation);
      float yOffset = lidar0Distance * sin(myLocation.getHeading() + lidar0Deviation);
      float xTmp = lidarSensors.measureLidar[0].RangeMilliMeter*cos(myLocation.getHeading()+lidar0Heading)+myLocation.getPosX()+xOffset;
      float yTmp = lidarSensors.measureLidar[0].RangeMilliMeter*sin(myLocation.getHeading()+lidar0Heading)+myLocation.getPosY()+yOffset;
      obstacle thisObstacle = checkIfObstacleExist(myObstacle, xTmp, yTmp);
      if(thisObstacle != NULL ){
        updateObstacle(thisObstacle, xTmp, yTmp);
      }
      else{
        addObstacle(myObstacle, xTmp, yTmp);
      }
    }
        if(lidarSensors.measureLidar[1].RangeMilliMeter <= maxRange){
      float xOffset = lidar1Distance * cos(myLocation.getHeading() + lidar1Deviation);
      float yOffset = lidar1Distance * sin(myLocation.getHeading() + lidar1Deviation);
      float xTmp = lidarSensors.measureLidar[1].RangeMilliMeter*cos(myLocation.getHeading()+lidar1Heading)+myLocation.getPosX()+xOffset;
      float yTmp = lidarSensors.measureLidar[1].RangeMilliMeter*sin(myLocation.getHeading()+lidar1Heading)+myLocation.getPosY()+yOffset;
      obstacle thisObstacle = checkIfObstacleExist(myObstacle, xTmp, yTmp);
      if(thisObstacle != NULL ){
        updateObstacle(thisObstacle, xTmp, yTmp);
      }
      else{
        addObstacle(myObstacle, xTmp, yTmp);
      }
    }
        if(lidarSensors.measureLidar[2].RangeMilliMeter <= maxRange){
      float xOffset = lidar2Distance * cos(myLocation.getHeading() + lidar2Deviation);
      float yOffset = lidar2Distance * sin(myLocation.getHeading() + lidar2Deviation);
      float xTmp = lidarSensors.measureLidar[2].RangeMilliMeter*cos(myLocation.getHeading()+lidar2Heading)+myLocation.getPosX()+xOffset;
      float yTmp = lidarSensors.measureLidar[2].RangeMilliMeter*sin(myLocation.getHeading()+lidar2Heading)+myLocation.getPosY()+yOffset;
      obstacle thisObstacle = checkIfObstacleExist(myObstacle, xTmp, yTmp);
      if(thisObstacle != NULL ){
        updateObstacle(thisObstacle, xTmp, yTmp);
      }
      else{
        addObstacle(myObstacle, xTmp, yTmp);
      }
    }
        if(lidarSensors.measureLidar[3].RangeMilliMeter <= maxRange){
      float xOffset = lidar3Distance * cos(myLocation.getHeading() + lidar3Deviation);
      float yOffset = lidar3Distance * sin(myLocation.getHeading() + lidar3Deviation);
      float xTmp = lidarSensors.measureLidar[3].RangeMilliMeter*cos(myLocation.getHeading()+lidar3Heading)+myLocation.getPosX()+xOffset;
      float yTmp = lidarSensors.measureLidar[3].RangeMilliMeter*sin(myLocation.getHeading()+lidar3Heading)+myLocation.getPosY()+yOffset;
      obstacle thisObstacle = checkIfObstacleExist(myObstacle, xTmp, yTmp);
      if(thisObstacle != NULL ){
        updateObstacle(thisObstacle, xTmp, yTmp);
      }
      else{
        addObstacle(myObstacle, xTmp, yTmp);
      }
    }
        if(lidarSensors.measureLidar[4].RangeMilliMeter <= maxRange){
      float xOffset = lidar4Distance * cos(myLocation.getHeading() + lidar4Deviation);
      float yOffset = lidar4Distance * sin(myLocation.getHeading() + lidar4Deviation);
      float xTmp = lidarSensors.measureLidar[4].RangeMilliMeter*cos(myLocation.getHeading()+lidar4Heading)+myLocation.getPosX()+xOffset;
      float yTmp = lidarSensors.measureLidar[4].RangeMilliMeter*sin(myLocation.getHeading()+lidar4Heading)+myLocation.getPosY()+yOffset;
      obstacle thisObstacle = checkIfObstacleExist(myObstacle, xTmp, yTmp);
      if(thisObstacle != NULL ){
        updateObstacle(thisObstacle, xTmp, yTmp);
      }
      else{
        addObstacle(myObstacle, xTmp, yTmp);
      }
    }
        if(lidarSensors.measureLidar[5].RangeMilliMeter <= maxRange){
      float xOffset = lidar5Distance * cos(myLocation.getHeading() + lidar5Deviation);
      float yOffset = lidar5Distance * sin(myLocation.getHeading() + lidar5Deviation);
      float xTmp = lidarSensors.measureLidar[5].RangeMilliMeter*cos(myLocation.getHeading()+lidar5Heading)+myLocation.getPosX()+xOffset;
      float yTmp = lidarSensors.measureLidar[5].RangeMilliMeter*sin(myLocation.getHeading()+lidar5Heading)+myLocation.getPosY()+yOffset;
      obstacle thisObstacle = checkIfObstacleExist(myObstacle, xTmp, yTmp);
      if(thisObstacle != NULL ){
        updateObstacle(thisObstacle, xTmp, yTmp);
      }
      else{
        addObstacle(myObstacle, xTmp, yTmp);
      }
    }
        if(lidarSensors.measureLidar[6].RangeMilliMeter <= maxRange){
      float xOffset = lidar6Distance * cos(myLocation.getHeading() + lidar6Deviation);
      float yOffset = lidar6Distance * sin(myLocation.getHeading() + lidar6Deviation);
      float xTmp = lidarSensors.measureLidar[6].RangeMilliMeter*cos(myLocation.getHeading()+lidar6Heading)+myLocation.getPosX()+xOffset;
      float yTmp = lidarSensors.measureLidar[6].RangeMilliMeter*sin(myLocation.getHeading()+lidar6Heading)+myLocation.getPosY()+yOffset;
      obstacle thisObstacle = checkIfObstacleExist(myObstacle, xTmp, yTmp);
      if(thisObstacle != NULL ){
        updateObstacle(thisObstacle, xTmp, yTmp);
      }
      else{
        addObstacle(myObstacle, xTmp, yTmp);
      }
    }
}