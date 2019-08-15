#include <Wire.h>
#include <TimerOne.h>
  
// I2C Arduino Nano Slave
  
int DHpin = 8;
byte dat[5];
  
char c[5];
float holesLeftWheel = 20;
float holesRightWheel = 20;
float counterLeft = 0;
float counterRight = 0;
float rotationLeft = 0;
float rotationRight = 0;
float speed_left = 0;
float speed_right = 0;
float distance = 0;
float interruptPeriod = 100000;
float frequency = 1.0/(interruptPeriod / 1000000);
//Convert float to bytes and bytes to float. 
//Has to be implemented on the other site too
union floatToBytes {
    char buffer[4];
    float number;
} converter;
  
void setup() {
    Wire.begin(0x70);
    Wire.onRequest(requestEvent); // data request to slave
    Wire.onReceive(receiveEvent); // data slave recieved 
    Serial.begin(115200);
    //pinMode(LED_BUILTIN, OUTPUT);
    Timer1.initialize(interruptPeriod);
    attachInterrupt(0,doCountLeft,RISING);
    attachInterrupt(1,doCountRight,RISING);
    Timer1.attachInterrupt ( timerInt);
}
  
void loop() {
    
}
//What to do if recive over I2C
void receiveEvent(int howMany) {
    // remember the question: H=humidity, T=temperature
    Serial.println("receiveEvent");
    int i = 0;
    while (0 < Wire.available()) {
        byte x = Wire.read();
        c[i] = x;
        i++;
    }
    if (c[0] == 'H'){
      for(int j = 0; j < 4; j++){
        converter.buffer[j] = c[j+1];
      }
      setHoleAmount(converter.number);
    }
}
//What to do if has to send over I2C
void requestEvent() {
  Serial.println("requestEvent");
  // respond to the question
  if (c[0] == 'S') {
    converter.number = (rotationLeft + rotationRight) /2;
  
    //byte response[] = { converter.buffer[0], converter.buffer[1], converter.buffer[2], converter.buffer[3]};
    Wire.write(converter.buffer, 4);
    Serial.println(converter.number);

    //digitalWrite(LED_BUILTIN, HIGH);
  }
}

//Count holes passed in left wheel
void doCountLeft(){
  counterLeft++;
  Serial.println("left");
}

//Count hole spassed in right wheel
void doCountRight(){
  counterRight++;
  Serial.println("right");
}

//Interrupt which gets called by timer.
//Calculates the rotations per period which is the rotations per second
void timerInt()
{
  Timer1.detachInterrupt();
  rotationLeft = (counterLeft/holesLeftWheel)*frequency;
  rotationRight = (counterRight/holesRightWheel)*frequency;
  counterLeft = 0;
  counterRight = 0;
  Timer1.attachInterrupt( timerInt );
}

//Set the hole amount if counterwheel is changed
void setHoleAmount(float num){
  holesLeftWheel = num;
  holesRightWheel = num;
}
  
