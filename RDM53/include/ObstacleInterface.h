/*
 * This file contains the informations for 
 * the obstacle interface 
 * 
 * 
 * Creation date: 2019 06 20
 * Author: Taha Tekdemir
 * Co-author: Jan Kühnemund
 */

#ifndef OBSTACLE_INTERFACE_H
#define OBSTABLE_INTERFACE_H

class ObstacleBuffer {
  public:
  int obstacleDetectionTime; // time, when the obstacle has been detected or updated
  int obstacleUpdateTime;
  int obstacleID; // temporary id of obstace
  int distanceObstacteToCar; // distance obstace to car
  int distanceToStartPosition;  // distance absolute to start position
  int angleToStartPosition; // angle absolute to start pos
  int angleObstacleToCar; // angle absolut to car
  float xCoordinate;
  float yCoordinate;
  float size;
};

class ObstacleInterface {
  public: 

  int maxRange = 1200;

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

  void initobstacledata();
  void checkForObstacles();
  void updateObstacle(obstacle, float, float);
  void addObstacle(obstacle, float, float);
  int getstartPosition();
  //int checkIfObstacleExist(int*);
  obstacle checkIfObstacleExist(obstacle, float, float);
  void fillObstacleStructure(int,int*,int*);

  private:

  int obstacleAmount;

};


struct obstBuff {
  ObstacleBuffer obstacles[30];
  int lastUpdated = 0;
};



#endif
