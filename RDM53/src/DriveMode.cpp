/*
 * This file contains the data/information for the driving
 * of the car 
 * 
 * 
 * Date: 2019 06 30
 * Author: Taha Tekdemir
 */

#include <DriveMode.h>
#include <EnginesInterface.h>
extern EnginesInterface engines;


void DriveMode::moveForward(){
    
    if(!goesForward){

        goesForward = true;
        engines.setEBL(0,255);
        engines.setEBR(0,255);
        engines.setEFR(0,255);
        engines.setEFL(0,255);
    }
}

void DriveMode::moveBackward(){

    goesForward = false;
    engines.setEBL(1,255);
    engines.setEBR(1,255);
    engines.setEFR(1,255);
    engines.setEFL(1,255);

}

void DriveMode::moveStop(){

    engines.setEBL(0,0);
    engines.setEBR(0,0);
    engines.setEFR(0,0);
    engines.setEFL(0,0);
    
}