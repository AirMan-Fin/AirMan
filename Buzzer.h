#ifndef BUZZER_H
#define BUZZER_H

#define dafsdf

#include "arduino.h"
enum Tone{
	C, D, E, F, G, A, H, C2,
	errorTone, okTone
};
///////tones     		  C2   H    A    G    F    E    D    C  ...
const uint8_t tone[] = { 255, 230, 210, 190, 170, 150, 130, 110, 90, 70, 50, 30,
		10 };

const uint8_t errorSound[]={
		C,D,E,F,G,A,H,C2
};

const uint8_t errorSoundLen[]={
		4,4,4,4,4,4,4,4
};

const uint8_t okSound[]={
		C,D,E,F,G,A,H,C2
};

const uint8_t okSoundLen[]={
		2,2,2,2,2,2,2,2
};

class Buzzer {
	PWMOutput *port;
	int timer;

	bool done;
	int bufferIndex;
	uint8_t ** buffer;
public:
	Buzzer(int p);

	/*
	 * plays tone or melody selected
	 * param: enum for tone/melody
	 */
	void play(Tone);

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
