/*
 * HCSR04P
 * based on:
 * https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib
 * 
 * modified by Pascal Pfeiffer
 * 2019 07 04
 */

#include "HCSR04P.h"

// init function
void HCSR04P::init(int out, int echo[], int n) {
	this->out = out;
	this->echo = echo;
	this->n = n;
	pinMode(this->out, OUTPUT);
	for(int i = 0; i < n; i++){ 
		pinMode(this->echo[i], INPUT); 
	}
}

// constructor one sensor
HCSR04P::HCSR04P(int out, int echo) {
	this->init(out, new int[1]{echo}, 1);
}

// constructor one sensor
HCSR04P::HCSR04P(int out, int echo, unsigned long timeOut) {
  this->init(out, new int[1]{echo}, 1);
  this->timeOut = timeOut;
}

// constructor multiple sensors
HCSR04P::HCSR04P(int out, int echo[], int n) {
	this->init(out, echo, n);
}

// destructor
HCSR04P::~HCSR04P() {
	~this->out;
	delete[] this->echo;
	~this->n;
}

// get distance of sensor n
int HCSR04P::dist(int n) const {
	digitalWrite(this->out, LOW);
	delayMicroseconds(2);
	digitalWrite(this->out, HIGH);
	delayMicroseconds(10);
	digitalWrite(this->out, LOW);
	
	noInterrupts();
	float d = pulseIn(this->echo[n], HIGH, timeOut);
	interrupts();
	return d / 5.8;
}

// get distance of sensor
int HCSR04P::dist() const {
	return this->dist(0);
}