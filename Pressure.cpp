/*
 * Pressure.cpp
 *
 *  Created on: 1.3.2016
 *      Author: Haggis
 */

#include "Pressure.h"
#define SCALE_FACTOR (240)

Pressure::Pressure(I2C *ii) {

	i2c=ii;
	Otemp[0] = 0xF1;


}

/*
 * returns pressure value
 * param: if true, doesn't read port, just return value
 */

bool Pressure::getValue(float * sss) {
	bool ret = 1;

	ret = i2c->transaction(0x40, Otemp, (uint32_t) 1, Itemp, (uint32_t) 0);
	delay(1);
	ret = i2c->transaction(0x40, Otemp, (uint32_t) 0, Itemp, (uint32_t) 3);


	value = (int16_t) Itemp[0] << 8 | Itemp[1];
	//printf("%3.2f \n",value);
	value = (value / SCALE_FACTOR) * 0.95;
	(*sss) = value;

	return ret;

}
