/*
 * This file contains the informations for 
 * the obstacle interface 
 * 
 * 
 * Date: 2019 06 20
 * Author: Taha Tekdemir
 */


class ObstacleBuffer {
  public:
  int obstacleDetectionTime; // time, when the obstacle has been detected or updated
  int obstacleID; // temporary id of obstace
  int distanceObstacteToCar; // distance obstace to car
  int distanceToStartPosition;  // distance absolute to start position
  int angleToStartPosition; // angle absolute to start pos
  int angleObstacleToCar; // angle absolut to car
};

class ObstacleInterface {
  public: 
  
  ObstacleInterface();

  void initobstacledata();
  int getstartPosition();
  int checkIfObstacleExist(int*);
  void fillObstacleStructure(int,int*,int*);
};


struct obstBuff {
  ObstacleBuffer obstacles[30];
  int lastUpdated = 0;
};