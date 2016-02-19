#ifndef HEATER_H
#define HEATER_H

#include "arduino.h"

class Heater{
private:
	int temp;
public:
	Heater();

	/*
	 * check if working as should
	 * param: new target temperature
	 */
	void update(int t=0);

	/*
	 * set target temperature for air in pipe near heater
	 */
	void setTemp(int);
};

#endif
