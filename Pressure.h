#ifndef PRESSURE_H
#define PRESSURE_H


#include "arduino.h"
#include "I2C.h"

class Pressure {
private:
	
	I2C* i2c;
	uint8_t Itemp[3];
	int Otemp[3];
	
public:
	/*
	 * creates new analogport object
	 */
	Pressure(int);

	/*
	 * returns pressure value
	 * param: if true, doesn't read port, just return value
	 */
	float getValue(bool read=0);

};

#endif
