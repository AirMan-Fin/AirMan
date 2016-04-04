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
#include "Eeprom.h"

#define pipeAreaMem 26
#define defaultPipeArea 0.3

/*
 * max cubicmeter per second fan can provide -> 500 hz frequency
 */
#define MAXAIRFLOW 0.2
#define MAXAIRSPEED 1.8
#define AIRSPEEDACCURACY 0.05
#define AUTOMODEDEFAULTSPEED 6


class Fan {
private:
	uint8_t calibration [102];
	int maxFlow;
	float flowLimit;
	float area;
	bool autoMode;
	Eeprom * eeprom;

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
	bool setFrequency( float);
public:
	Fan(Millis *m ,Eeprom *ee,int s = 0);

	/*
	 * hearthbeat function
	 */
	bool update(float);

	/*
	 * makes calculations and sets airflow
	 * params: airflow, pressure in ducts
	 */
	int setAirFlow(float, float);

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

	/*
	 * sets pipe output area
	 */
	void setPipeArea(float ar);

	float getPipeArea();

	void reset(bool b=0);



};

#endif
