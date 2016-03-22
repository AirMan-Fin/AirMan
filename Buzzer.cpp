#include "Buzzer.h"

Buzzer::Buzzer(int p) {
	port = new PWMOutput(p);

}

/*
 * plays tone or melody selected
 * param: enum for tone/melody
 */
void Buzzer::play(Tone t) {
	switch (t) {
	case errorTone:
		buffer = new uint8_t*[9];
		for (int i = 0; i < 9; ++i)
			buffer[i] = new uint8_t[2];
		bufferIndex = 8;
		timer = 1;
		for (int a = bufferIndex; a > 0; a--) {
			buffer[a][0] = errorSoundLen[a];
			buffer[a][1] = errorSound[a];
		}
		break;
	case okTone:
		buffer = new uint8_t*[9];
		for (int i = 0; i < 9; ++i)
			buffer[i] = new uint8_t[2];
		bufferIndex = 8;
		timer = 1;
		for (int a = bufferIndex; a > 0; a--) {
			buffer[a][0] = okSoundLen[a];
			buffer[a][1] = okSound[a];
		}
		break;
	}
}

void Buzzer::beeb(int len) {
	buffer = new uint8_t*[2];
	for (int i = 0; i < 2; ++i)
		buffer[i] = new uint8_t[2];
	bufferIndex = 1;
	buffer[1][0] = len;
	buffer[1][1] = 255;
}

void Buzzer::tick() {
	if (timer > 0) {
		timer--;
		if (timer == 0) {
			if (bufferIndex > 0) {
				timer = buffer[bufferIndex][0];
				port->write(buffer[bufferIndex][1]);
				bufferIndex--;
			} else {
				port->write(0);
			}

		}
	}

}

