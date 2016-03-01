/*
 * Fan.cpp
 *
 *  Created on: 1.3.2016
 *      Author: Haggis
 */

#include "Fan.h"

Fan::Fan(int fanspeed) {
	speed = fanspeed;
	node = new ModbusMaster (2); // Create modbus object that connects to slave id 2
	node->begin(9600);
}

void Fan::update(int pulse) {
	//Chip_GPIO_SetPinState(LPC_GPIO, port, pin, true);
}

bool Fan::setFrequency(uint16_t freq) {
	uint8_t result;
	int ctr;
	bool atSetpoint;
	const int delayy = 500;

	node->writeSingleRegister(1, freq); // set motor frequency

	//printf("Set freq = %d\n", freq/40); // for debugging

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		delay(delayy);
		// read status word
		result = node->readHoldingRegisters(3, 1);
		// check if we are at setpoint
		if (result == node->ku8MBSuccess) {
			if (node->getResponseBuffer(0) & 0x0100)
				atSetpoint = true;
		}
		ctr++;
	} while (ctr < 20 && !atSetpoint);

	//printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}

void Fan::setAirFlow(int flow){
	float f= flow;
	f/MAXAIRFLOW *5;
	setFrequency((uint16_t)f);
}


