/*
 * Pressure.cpp
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
float Pressure::getValue(bool b = 0){
	if (!b) {
			float pressure = port->read();
			float scale = (pressure / /*SCALE FACTOR*/);
			float edp = pressure * 0.95;
			
			

		}

}
