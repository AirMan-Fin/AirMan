#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "arduino.h"

class Temperature {
private:
	AnalogPort *port;
	float value;
public:
	/*
	 * creates new analogport object
	 */
	Temperature(int);

	/*
	 * returns port value
	 * param: if true, doesn't read port, just return value
	 */
	float getValue(bool read=0);

};

#endif
