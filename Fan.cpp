/*
 * Fan.cpp
 *
 *  Created on: 1.3.2016
 *      Author: Haggis
 */

#include "board.h"
#include "arduino.h"
#include "Fan.h"

Fan::Fan(int fanspeed = 0) {
	speed = fanspeed;
}

void Fan::update(int pulse){
	Chip_GPIO_SetPinState(LPC_GPIO, port, pin, true);
}

bool Fan::setFrequency(ModbusMaster& node, uint16_t freq) {
	uint8_t result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	node.writeSingleRegister(1, freq); // set motor frequency

	//printf("Set freq = %d\n", freq/40); // for debugging

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = node.readHoldingRegisters(3, 1);
		// check if we are at setpoint
		if (result == node.ku8MBSuccess) {
			if(node.getResponseBuffer(0) & 0x0100) atSetpoint = true;
		}
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	//printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}


