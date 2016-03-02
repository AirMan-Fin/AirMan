/*
 * Pressure.cpp
 *
 *  Created on: 1.3.2016
 *      Author: Haggis
 */

#include "Pressure.h"
#define SCALE_FACTOR (1/60)

Pressure::Pressure(int a){

	i2c = new I2C(0, 100000);
	Otemp[0] = 0xF1;

}

/*
 * returns pressure value
 * param: if true, doesn't read port, just return value
 */


float Pressure::getValue(bool b = 0){
	if (!b) {
		
		i2c.transanction(0x40, &Otemp, 1, &Itemp, 3);
		float luku = Itemp[0] << 8 | Itemp[1];
	
		luku = (luku / SCALE_FACTOR);
		luku = scale * 0.95;
			
		return luku;

		}
	else return luku;

}
