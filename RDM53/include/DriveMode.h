/*
 * This file contains the data/information for the driving
 * of the car 
 * 
 * 
 * Date: 2019 06 30
 * Author: Taha Tekdemir
 */

class DriveMode {
    public:


    const int MAXIMUM_DISTANCE = 500; //in mm
    const int MINIMUM_DISTANCE = 100; // in mm
    int distance = 0;
    int distanceRight;
    int distanceLeft;
    bool goesForward = false;
    bool freeSpace = false;

    void moveForward();
    void moveBackward();
    void moveStop();
    void turnRight();
    void turnLeft();

    void lookLeft(); // looking if there is any space in front left side of the car 
    void lookRight(); // looking if there is any space in front right side of the car 
    void readSensorData();


};