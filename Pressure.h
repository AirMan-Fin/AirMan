#ifndef PRESSURE_H
#define PRESSURE_H


#include "arduino.h"

class Pressure {
private:
	AnalogPort *port;
	float value;
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
