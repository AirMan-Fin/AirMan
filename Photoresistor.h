#ifndef PHOTORESISTOR_H
#define PHOTORESISTOR_H

using namespace std;

//#include "arduino.h"
#include <iostream>
#include <algorithm>

class Photoresistor {
private:
	//AnalogPort *sensor;
	int counter;

	int photoArray[10];
	int photoIndex;
	int resistorArray[144];
	int resistorIndex;

	int upperAverage;
	int lowerAverage;

	int threshold;
	bool isLightOn;

public:
	
	Photoresistor(int);

	bool getLightState();
	bool update(float *);
	void measure();

};

#endif
