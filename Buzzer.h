#ifndef BUZZER_H
#define BUZZER_H

#define dafsdf

#include "arduino.h"

class Buzzer {
	Output *port;
	int timer;
public:
	Buzzer(int p);

	/*
	 * makes one beeb
	 * param: length of the beeb
	 *
	 */
	void beeb(int length = 0.5);

	/*
	 * counting sound lengths
	 */
	void tick();

};

#endif
