/*
 * HCSR04P
 * based on:
 * https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib
 * 
 * modified by Pascal Pfeiffer
 * 2019 07 04
 */

#include <Arduino.h>
class HCSR04P
{
	public:
    // initialisation class HCSR04 (trig pin , echo pin)
	HCSR04P(int out,int echo);
    // initialisation class HCSR04 (trig pin , echo pin, timeOut)
	HCSR04P(int out,int echo, unsigned long timeOut);
    // initialisation class HCSR04 (trig pin , echo pin)
	HCSR04P(int out,int echo[],int n);		
    // destructor
	~HCSR04P();

    // return curent distance of element 0
	int dist() const;

    // return curent distance of element n
	int dist(int n) const;				

	private:
    // initialisation of the required µC and sensor HW
	void init(int out,int echo[],int n);
    // out pin "trigger"
	int out;
    // echo pin list
	int *echo;
    // number of el
	int n;									
    // default timeout for ~ 1 meter
	unsigned long timeOut = 6000;			
};