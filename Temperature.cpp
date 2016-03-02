#include "Temperature.h"
#include <cstdio>

Temperature::Temperature(int a) {
	port = new AnalogPort(a);
}

/*
 * 0 celcius = 1603 ohm
 * 11 ohm per celcius above that, max 40 c
 */
const float offset = -450;
bool Temperature::getValue(float * res) {
	float aa = port->read();
	aa = (aa / 4095) * 5;
	aa = (1500 * aa) / (5 - aa);
	aa += offset;
	value = (aa - 1603) / 11;
	//printf("aa=%d, tt=%d\n", (int) aa, (int) value);
	(*res) = value;

	float ttt = (lastResults[0]+ lastResults[1]+ lastResults[2]) / 3;
	lastResults[0]=lastResults[1];
	lastResults[1]=lastResults[2];
	lastResults[2]=value;
	if((ttt - value) > -5 && (value -ttt) > -5){
		return 1;
	}
	else return 0;

}
