#include "Buzzer.h"

Buzzer::Buzzer(int p) {
	port = new Output(p);

}



void Buzzer::beeb(int len) {
	timer=len;
	port->write(1);
}

void Buzzer::tick() {
	if (timer > 0) {
		timer--;
		if (timer == 0) {
			port->write(0);
		}
	}

}

