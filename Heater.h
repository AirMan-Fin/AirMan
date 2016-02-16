#ifndef HEATER_H
#define HEATER_H

#include "arduino.h"

class Heater{
private:
	bool autoMode;
	int temp;
public:
	Heater();

	void update(int);
	void setTemp();
};

#endif
