#ifndef PHOTORESISTOR_H
#define PHOTORESISTOR_H

using namespace std;

#include "arduino.h"
#include <algorithm>

class Photoresistor {
private:
	AnalogPort *sensor;
	int counter;

	int photoArray[10];
	int photoIndex;
	int resistorArray[144];
	int resistorIndex;

	int upperAverage;
	int lowerAverage;

	int accuracy;
public:
	
	Photoresistor(int);

	/*
	 * returns port value
	 * param: if true, doesn't read port, just return value
	 */
	bool update(float *);
	void measure();

};

#endif
