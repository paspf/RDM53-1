
#include "ObstacleTracker.h"
#include "location.h"
#include <lidar.h>
#include <HCSR04P.h>


extern Location myLocation;

extern lidar lidarSensors; //lidar object to work directly with the sensor data (RangeMillimeter)
//extern obstBuff obstBuffer;

extern HCSR04P ultraSonic;

//detect the obstacledata to the car 

/*void ObstacleInterface::setobstacleID(int oID){

    obstacleID = oID;
}
 */

/**
 * This function checks all obstacles if the detected obstacle is within a square with a side lenght of 100mm of a previously
 * detected ostacle.
 * If it is within a square of a previous object the previous object gets returned.
 * if not a pointer to NULL gets returned.
 */
ObstacleTracker::obstacle ObstacleTracker::checkIfObstacleExist(obstacle currentObstacle, float xTmp, float yTmp){
  while (currentObstacle != NULL){
    float xDiff = currentObstacle->xCoordinate - xTmp;
    if (xDiff < 100 && xDiff > -100){
        float yDiff = currentObstacle->yCoordinate - yTmp;
        if(yDiff < 100  && yDiff > -100){
          return currentObstacle;
        }
    }
    currentObstacle = currentObstacle->next;
  }
  return NULL;
}
/**
 * This function updates Obstacle Objects. it takes the object to update, an x and y float and 
 * calculates the new position as well as a new radius. It also adds the last time updated.
 */
void ObstacleTracker::updateObstacle(obstacle thisObstacle, float xTmp, float yTmp){
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
/**
 * This function adds Obstacle Objects. It takes a pointer to the previous Obstacle Object and adds 
 * values to the new Obstacle Object. These values are detection time, x/yCoordinate, a basic rdaius of 20 mm
 * a pointer to the next Object (Null).
 * It also adds the pointer to the new Object to the previous Object and increases the total Object amount
 */
void ObstacleTracker::addObstacle(obstacle thisObstacle, float xTmp, float yTmp){
  obstacle newObstacle = (obstacle) malloc(sizeof(struct obstacle_struct));
  while (thisObstacle->next != NULL){
    thisObstacle = thisObstacle->next;
  }
  newObstacle->detectionTime = millis();
  newObstacle->xCoordinate = xTmp;
  newObstacle->yCoordinate = yTmp;
  newObstacle->radius = 20;
  newObstacle->next = thisObstacle->next; //NULL
  thisObstacle->next = newObstacle;

  obstacleAmount++;
}
/**
 * This function merge sorts the Obstacle Object List
 */
void ObstacleTracker::mergeSort(obstacle* thisObstacle){
  obstacle head = *thisObstacle;
  obstacle a;
  obstacle b;

  if ((head == NULL)| (head->next == NULL)){
    return;
  }
  frontBackSplit(head, &a,&b);

  mergeSort(&a);
  mergeSort(&b);

  *thisObstacle = sortedMerge(a,b);
}
/**
 * This function compares two Objects by distance to the car and sorts them. 
 * It changes the pointers in a way that the object with a further distance is further down the list.
 * It then returns the pointer to the object which is nearer.
 */
ObstacleTracker::obstacle ObstacleTracker::sortedMerge(obstacle a, obstacle b)
{
  obstacle result = NULL;

  if ( a == NULL){
    return (b);
  }
  else if (b == NULL){
    return (a);
  }
  float distance_a = sqrtf(pow(a->xCoordinate-myLocation.getPosX(),2)+pow(a->yCoordinate-myLocation.getPosY(),2));
  float distance_b = sqrtf(pow(b->xCoordinate-myLocation.getPosX(),2)+pow(b->yCoordinate-myLocation.getPosY(),2));
  if(distance_a <= distance_b){
    result = a;
    result->next=sortedMerge(a->next, b);
  }
  else{
    result = b;
    result->next = sortedMerge(a, b->next);
  }
  return (result);
}
/**
 * Thanks to GeeksforGeeks for their amazing example.
 * This function splits the nodes of a given list into front and back halves. If size is odd,
 * the extra node foes to front list.
 * Source: https://www.geeksforgeeks.org/merge-sort-for-linked-list/
 */
void ObstacleTracker::frontBackSplit(obstacle thisObstacle, obstacle* frontRef, obstacle* backRef){
  obstacle fast;
  obstacle slow;
  slow = thisObstacle;
  fast = thisObstacle->next;

  while (fast != NULL) { 
        fast = fast->next; 
        if (fast != NULL) { 
            slow = slow->next; 
            fast = fast->next; 
        } 
    } 
  
    /* 'slow' is before the midpoint in the list, so split it in two  
    at that point. */
    *frontRef = thisObstacle; 
    *backRef = slow->next; 
    slow->next = NULL; 
}
/**
 * This function deletes the last object from the list and frees the memory taken up by that object.
 */
void ObstacleTracker::delDistObst(obstacle thisObstacle)
{
  while(thisObstacle->next->next != NULL){
    thisObstacle = thisObstacle->next;
  }
  obstacle tmp = thisObstacle->next;
  free(tmp);
  obstacleAmount--;
}
/**
 * This function checks sensor data and adds/updates an object if necessary.
 * It also deletes the object furthest away if there are more objects than maxObstacleAmount.
 */
void ObstacleTracker::checkForObstacles(){
  if(obstacleAmount > maxObstacleAmount){
    mergeSort(&myObstacle);
    delDistObst(myObstacle);
  }

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
