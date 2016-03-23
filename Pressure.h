#ifndef PRESSURE_H
#define PRESSURE_H


#include "arduino.h"
#include "I2C.h"
#include "cstdio"

class Pressure {
private:

	float value;
	float edValue;
	I2C* i2c;
	uint8_t Itemp[3];
	uint8_t Otemp[3];

public:
	/*
	 * creates new analogport object
	 */
	Pressure(I2C *);

	/*
	 * returns pressure value
	 * param: if true, doesn't read port, just return value
	 */
	bool getValue(float *);

};

#endif
