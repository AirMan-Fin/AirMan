/*
 * Pressure.cpp
 *
 *  Created on: 1.3.2016
 *      Author: Haggis
 */

#include "Pressure.h"
#define SCALE_FACTOR (240.0)

Pressure::Pressure(I2C *ii) {

	i2c=ii;
	Otemp[0] = 0xF1;
	value=0;
	edValue=0;

}

/*
 * returns pressure value
 * param: if true, doesn't read port, just return value
 */

bool Pressure::getValue(float * sss) {

	i2c->transaction(0x40, Otemp, (uint32_t) 1, Itemp, (uint32_t) 0);
	delay(5);
	bool ret = i2c->transaction(0x40, Otemp, (uint32_t) 0, Itemp, (uint32_t) 3);
	value = (int16_t) (Itemp[0] << 8 | Itemp[1]);
	value = -((value / SCALE_FACTOR) * 0.5);
	if(value<0){
		value=0;
	}
	if((value - edValue)<20 && (edValue - value)<20){
	value= edValue+((value - edValue)*0.2);
	}
	(*sss) = value;
	edValue=value;
	//printf("value: %3.2f \n",value);
	return ret;

}
