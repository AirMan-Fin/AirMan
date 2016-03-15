#ifndef PRESSURE_H
#define PRESSURE_H


#include "arduino.h"
#include "I2C.h"

class Pressure {
private:

	int value;
	I2C* i2c;
	uint8_t Itemp[3];
	uint8_t Otemp[3];

public:
	/*
	 * creates new analogport object
	 */
	Pressure(int);

	/*
	 * returns pressure value
	 * param: if true, doesn't read port, just return value
	 */
	bool getValue(float *);

};

#endif
