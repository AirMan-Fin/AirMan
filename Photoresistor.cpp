#include "Photoresistor.h"
#include <cstdio>

Photoresistor::Photoresistor(int a) {
	sensor = new AnalogPort(a);
	counter = 0;
	photoIndex = 0;
	resistorIndex = 0;
}



bool Photoresistor::update(float * res) {

	counter++;

	if (counter > 60) {
		counter = 0;
		
		photoArray[photoIndex] = sensor->read();
		photoIndex++;
		
		if (photoIndex == 9) {
			photoIndex = 0;
			int gg;
			for (int i = 0; 1 < 10; i++){

			}
			resistorArray[resistorIndex] = ggg;
			resistorIndex++;
		}

	}

		if ((/*sekoilu1*/) > 4095 && (/*sekoilu2*/) > 4095){

			return 1;
		}

		else return 0;
	}
}
