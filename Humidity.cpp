/*
 * Humidity.cpp
 *
 *  Created on: 1.3.2016
 *      Author: Haggis
 */

#include "Humidity.h"
#include <cstdio>

Humidity::Humidity(int a) {

	i2c = new I2C(0, 100000);
	Otemp[0] = 0xF1;

}

/*
 * returns humidity value
 * param: if true, doesn't read port, just return value
 */

bool Humidity::getValue(float * res) {
	bool ok = i2c->transaction(0x27, Otemp, (uint32_t) 1, Itemp, (uint32_t) 3);
	float luku = (Itemp[0] & 0b00111111) << 8 | Itemp[1];
	//printf("status: %d, dd: %d\n", (Itemp[0] >> 6), (int)luku);

	luku = (luku / 16386) * 100;

	value = luku;
	(*res) = value;
	return ok;

}
