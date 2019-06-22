/*
 * This file contains the informations for 
 * the obstacle interface 
 * 
 * 
 * Date: 2019 06 20
 * Author: Taha Tekdemir
 */


class ObstacleInterface {
  public: 
  ObstacleInterface();

  int obstacleID;
  int distancecar;
  int distancestart;
  int anglestart;
  int angleofobstacle;
  
  
  int initobstacleposition();
  int initobstacledistance();
  void setobstacleID(int);

 
};