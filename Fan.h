#ifndef FAN_H
#define FAN_H

/*
* Fan.h
*
*  Created on: 1.3.2016
*      Author: Haggis
*/

#include "arduino.h"
#include "ModbusMaster.h"
#include "cstdio"

/*
 * max cubicmeter per second fan can provide -> 500 hz frequency
 */
#define MAXAIRFLOW 10

class Fan {
private:
	bool autoMode;
	int speed;
	ModbusMaster *node;

	bool setFrequency( uint16_t);
public:
	Fan(Millis *m ,int s = 0);

	void update();

	bool setAirFlow(int);

};

#endif
