#include "Temperature.h"
#include <cstdio>

Temperature::Temperature(int a) {
	port = new AnalogPort(a);
	getValue();
}

/*
 * 0 celcius = 1603 ohm
 * 11 ohm per celcius above that, max 40 c
 */
const float offset=-450;
float Temperature::getValue(bool b) {
	if (!b) {

		float aa = port->read();
		aa = (aa / 4095) * 5;
		aa = (1500 * aa) / (5 - aa);
		aa+=offset;
		value=(aa-1603)/11;
		//printf("aa=%d, tt=%d\n", (int) aa, (int) value);

	}
	return value;
}
