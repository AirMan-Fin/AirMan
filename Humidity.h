#ifndef HUMIDITY_H
#define HUMIDITY_H


#include "arduino.h"
#include "I2C.h"

class Humidity {
private:

	I2C* i2c;
	uint8_t Itemp[4];
	uint8_t Otemp[2];

	float tempValue;
	float humiValue;

public:
	/*
	* creates new analogport object
	*/
	Humidity(I2C*);

	/*
	* returns pressure value
	* param: if true, doesn't read port, just return value
	*/
	bool getValue(float *, float *);

};

#endif
