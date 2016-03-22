/*
 * Humidity.cpp
 *
 *  Created on: 1.3.2016
 *      Author: Haggis
 */

#include "Humidity.h"
#include <cstdio>

Humidity::Humidity(I2C *ii) {

	i2c = ii;


}

/*
 * returns humidity value
 * param: if true, doesn't read port, just return value
 */

bool Humidity::getValue(float * res, float * res2) {
	i2c->plainWriteCommand(0x27);
	delay(40);
	bool ok = 1;
	ok = i2c->transaction(0x27, Otemp, (uint32_t) 0, Itemp, (uint32_t) 4);

	float luku = (Itemp[0] & 0b00111111) << 8 | Itemp[1];
	humiValue = (luku / 16386);

	float luku2 = ((Itemp[2] << 8) | Itemp[3]) >> 2;
	tempValue = (luku2 / 16386) * 165 - 40;

	//printf("status: %d, temp: %3.2f, humi:%3.2f\n", (Itemp[0] >> 6), tempValue, humiValue);

	(*res) = humiValue;
	(*res2) = tempValue;
	return ok;

}
