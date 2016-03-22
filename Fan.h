#ifndef FAN_H
#define FAN_H

/*
* Fan.h
*
*  Created on: 1.3.2016
*      Author: Haggis
*/


//register 102 for reading frequency

#include "arduino.h"
#include "ModbusMaster.h"
#include "cstdio"

/*
 * max cubicmeter per second fan can provide -> 500 hz frequency
 */
#define MAXAIRFLOW 0.2
#define MAXAIRSPEED 1.5
#define AIRSPEEDACCURACY 0.1


class Fan {
private:
	uint8_t calibration [102];
	int maxFlow;
	float flowLimit;
	float area;
	bool autoMode;

	float targetAirSpeed;
	float currentAirSpeed;
	float frequency;

	ModbusMaster *node;

	/*
	 * function to calculate airvelocity
	 * params: pressure
	 * returns velocity
	 */
	float getAirVelocity(float);

	/*
	 * sends frequency to modbus
	 */
	bool setFrequency( uint16_t);
public:
	Fan(Millis *m ,int s = 0);

	/*
	 * hearthbeat function
	 */
	void update(float);

	/*
	 * makes calculations and sets airflow
	 * params: airflow, pressure in ducts
	 */
	int setAirFlow(int, float);

	/*
	 * sets calibration point for airflow
	 * params: point, flow
	 */
	void setFlowPoint(int point=0, int fl =0);


	/*
	 * sets automode
	 * param: automode on or off
	 */
	void setAutoMode(bool);




};

#endif
