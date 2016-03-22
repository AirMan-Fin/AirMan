/*
 * Fan.cpp
 *
 *  Created on: 1.3.2016
 *      Author: Haggis
 */

#include "Fan.h"

Fan::Fan(Millis *m, int fanspeed) {
	for (int a = 0; a < 100; a++) {
		calibration[a]=0;
	}
	autoMode = 1;

	node = new ModbusMaster(m, 2); // Create modbus object that connects to slave id 2
	node->begin(9600);

	node->writeSingleRegister(0, 0x0406);
	delay(1000);
	node->writeSingleRegister(0, 0x047F);
	delay(1000);

}

void Fan::update(float pres) {

	currentAirSpeed = getAirVelocity(pres);
	if (currentAirSpeed - targetAirSpeed < AIRSPEEDACCURACY
			&& targetAirSpeed - currentAirSpeed < AIRSPEEDACCURACY) {
		if (currentAirSpeed < targetAirSpeed) {
			frequency += 10;
		} else {
			frequency -= 10;
		}
	} else {
		calibration[((int) frequency) / 5] = (uint8_t) (currentAirSpeed * 100);
	}

	setFrequency(frequency);
}

bool Fan::setFrequency(uint16_t freq) {
	bool ok = 0;

	uint8_t result = node->writeSingleRegister(1, freq);

	if (result == node->ku8MBSuccess) {
		ok = 1;
	}

	return ok;
}

int Fan::setAirFlow(int flow, float pres) {
	bool ret = 0;
	if (autoMode) {
		currentAirSpeed = getAirVelocity(pres);
		targetAirSpeed = flow / area;
		if (targetAirSpeed > MAXAIRSPEED) {
			ret += 10;
			targetAirSpeed = MAXAIRSPEED;
		}
		frequency = (targetAirSpeed * area) / (currentAirSpeed * area) * 500;
	} else {
		frequency = calibration[(int)((targetAirSpeed/MAXAIRSPEED)*100)];
	}
	//printf("%d\n", targetAirSpeed * area);

	if (!setFrequency((uint16_t) frequency)) {
		ret += 1;
	}
	return ret;
}

/*
 * function to calculate airvelocity
 * params: pressure
 * returns velocity
 */
float Fan::getAirVelocity(float pressure) {
	return (pressure * area) / 1.205;

}

void Fan::setFlowPoint(int point, int fl) {
	if (point == 0) {
		maxFlow = fl;
		flowLimit = MAXAIRFLOW / (float) fl;
	} else {
		calibration[point] = fl;
	}
}

void Fan::setAutoMode(bool mo) {
	autoMode = mo;
}

/*
 *
 *0, 0x047F
 *
 */

