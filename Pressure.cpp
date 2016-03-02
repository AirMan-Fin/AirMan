/*
 * Fan.cpp
 *
 *  Created on: 1.3.2016
 *      Author: Haggis
 */

#include "Pressure.h"

Pressure::Pressure(int a){
	port = new AnalogPort(a);
	getValue();

}

/*
 * returns pressure value
 * param: if true, doesn't read port, just return value
 */
float Pressure::getValue(bool b ){
	if (!b) {
			float aa = port->read();
			aa = (aa / 4095) * 5;
			aa = (1500 * aa) / (5 - aa);

			value=(aa-1603)/11;
			//printf("aa=%d, tt=%d\n", (int) aa, (int) value);

		}
	return 1;

}
