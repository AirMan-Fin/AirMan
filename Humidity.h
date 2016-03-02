#ifndef HUMIDITY_H
#define HUMIDITY_H


#include "arduino.h"
#include "I2C.h"

class Humidity {
private:

	I2C* i2c;
	uint8_t Itemp[3];
	int Otemp[3];

public:
	/*
	* creates new analogport object
	*/
	Humidity(int);

	/*
	* returns pressure value
	* param: if true, doesn't read port, just return value
	*/
	float getValue(bool read = 0);

};

#endif
