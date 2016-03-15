/*
 * Fan.cpp
 *
 *  Created on: 1.3.2016
 *      Author: Haggis
 */

#include "Fan.h"

Fan::Fan(Millis *m, int fanspeed) {
	speed = fanspeed;
	node = new ModbusMaster(m, 2); // Create modbus object that connects to slave id 2
	node->begin(9600);

	node->writeSingleRegister(0, 0x0406);

	delay(1000);

	node->writeSingleRegister(0, 0x047F);

	delay(1000);
}

void Fan::update() {
	node->writeSingleRegister(0, 0x0406);
}

bool Fan::setFrequency(uint16_t freq) {
	uint8_t result;
	int ctr;
	bool ok = 0;
	uint16_t data[6];

	//node->setTransmitBuffer(0,255);

	result = node->writeSingleRegister(1, freq);


	if (result == node->ku8MBSuccess) {
		ok = 1;
	}

	return ok;
}

bool Fan::setAirFlow(int flow) {
	float f = flow;
	f = 60;
	return setFrequency((uint16_t) f);
}

/*
 *
 *0, 0x047F
 *
 */

