# RDM53-1
RDM53 is a university project with the aim of developing an autonomous vehicle that manages a course with obstacles and recognizes its goal.
The obstacles have a minimum size of 10*10cm, the course limits as well as the target is marked by colored adhesive stripes on the floor.

| function  | adhesive stripe |
| ------------- | ------------- |
| black | course border |
| green | goal |
| yellow | prohibited area |

# Software
The software on the RDM53 is divided into several layers. In the lowest layer the sensor data is read and the engines are controlled. In the upper layers the sensor data is processed, the autonomy and the communication with the user takes place. 

## Setup Wi-Fi
The Wi-Fi network is configured in the mainUSR.h file. The defined network type (Home/Enterprise) as well as the login data can be viewed here.

## Our Setup
The folder RDM contains the main project created with Visual Studio Code and PlatformIO. The Folder ArduinoSpeedSensor contains some software for testing the speed sensors. This software is not required for the main project. The Folder MagneticCalibration contains a MATLAB tool, which can be used to calibrate the MPU9250. Last but not least there is a Webinterface which can be used to read out sensor values or to control the RDM by remote (e.g. with an XBOX controller).

## Communicate with the RDM53
Commands can be transmitted to the RDM53 via serial connection and as well via Websockets. The binary stream is set as follows:
![Alt text](Images/protocol.png?raw=true "RDM53 Protocol")

# Hardware
RDM53 uses a wide variety of sensors to realize the autonomous functions.
The core of the RDM53 is an ESP32 on a WeMos LolinD32 board. In addition to the ESP32 this microcontroller board contains a charging control system that is used for lithium batteries.

| Component  | Type | Function |
| ------------- | ------------- | ------------- |
| WeMos LolinD32  | microcontroller board | brain of the RDM53 |
| PCF85741  | port expansion | - |
| DRV8833  |  dual H-Bridge motor driver | power electronic |
| GRV IMU 10 DOF v2  | MPU9250 board | accelerometer, gyroscope, magnetometer |
| TCRT5000 | reflective optical sensor |coarse border detection (stripe detection) |
| VL53L0X | laser ranging sensor | obstacle detection |
| HC-SR04P | ultrasonic sensor | obstacle detection |
| TCS34725 | color sensor | adhesive stripe color detection |
| Joy-It Speed sensor | speed sensor | speed calculation |
| Joy-It Robot Car Kit 4WD | scaffold | base of the RDM53 |

## Schematic
![Alt text](Images/schematic.png?raw=true "RDM53 Schematic")

## The final vehicle
![Alt text](Images/RDM53-1.jpg?raw=true "RDM53 Vehicle")
