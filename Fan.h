#ifndef FAN_H
#define FAN_H

#include "arduino.h"
#include "ModbusMaster.h"

/*
 * max cubicmeter per second fan can provide -> 500 hz frequency
 */
#define MAXAIRFLOW 100

class Fan {
private:
	bool autoMode;
	int speed;
	ModbusMaster *node;

	bool setFrequency( uint16_t);
public:
	Fan(int s = 0);

	void update(int);

	void setAirFlow(int);

};

#endif
