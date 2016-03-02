#ifndef HUMIDITY_H
#define HUMIDITY_H


#include "arduino.h"
#include "I2C.h"

class Humidity {
private:

	I2C* i2c;
	uint8_t Itemp[3];
	uint8_t Otemp[3];

	int value;

public:
	/*
	* creates new analogport object
	*/
	Humidity(int);

	/*
	* returns pressure value
	* param: if true, doesn't read port, just return value
	*/
	bool getValue(float *);

};

#endif
