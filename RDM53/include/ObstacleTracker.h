/*
 * ObstacleTracker detects and tracks  obstacles
 * Lidar and magnetometer is used
 * 
 * THIS FUNCTION IS UNTESTED!!
 * 
 * Creation date: 2019 08 08
 * Author: Jan Kühnemund
 */

#ifndef OBSTACLETRACKER_H
#define OBSTACLETRACKER_H

class ObstacleTracker {
  public:
  short maxRange = 1200;
  short maxObstacleAmount = 50;

  float ultraSonicDeviation = 0; //Degrees to Heading when drawing Line from RDMcenter
  float ultraSonicHeading = 0; //Degrees Deviation from Heading of RDM
  float ultraSonicDistance = 140; //mm distance from center of RDM

  float lidar0Deviation = 335;
  float lidar0Heading = 0;
  float lidar0Distance = 155;

  float lidar1Deviation = 345;
  float lidar1Heading = 330;
  float lidar1Distance = 170;

  float lidar2Deviation = 15;
  float lidar2Heading = 30;
  float lidar2Distance = 170;

  float lidar3Deviation = 25;
  float lidar3Heading = 0;
  float lidar3Distance = 155;

  float lidar4Deviation = 90;
  float lidar4Heading = 90;
  float lidar4Distance = 105;

  float lidar5Deviation = 180;
  float lidar5Heading = 180;
  float lidar5Distance = 140;

  float lidar6Deviation = 270;
  float lidar6Heading = 270;
  float lidar6Distance = 105;

  struct obstacle_struct {
    int detectionTime;
    int updateTime;
    float radius;
    float xCoordinate;
    float yCoordinate;
    struct obstacle_struct *next;
  };
  typedef struct obstacle_struct* obstacle;

  void checkForObstacles();
  void delDistObst(obstacle);
  void mergeSort(obstacle*);
  obstacle sortedMerge(obstacle, obstacle);
  void frontBackSplit(obstacle, obstacle*, obstacle*);
  void updateObstacle(obstacle, float, float);
  void addObstacle(obstacle, float, float);
  //int checkIfObstacleExist(int*);
  obstacle checkIfObstacleExist(obstacle, float, float);


  private:

  int obstacleAmount;
  obstacle myObstacle;
};

#endif