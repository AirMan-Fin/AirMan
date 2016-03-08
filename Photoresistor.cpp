#include "Photoresistor.h"

Photoresistor::Photoresistor(int a) {
	sensor = new AnalogPort(a);
	counter = 0;
	photoIndex = 0;
	resistorIndex = 0;
}

bool Photoresistor::measure() {

	counter++;

	if (counter > 60) {
		counter = 0;

		photoArray[photoIndex] = sensor->read();
		photoIndex++;

		if (photoIndex == 9) {
			photoIndex = 0;
			int temp = 0;
			for (int i = 0; 1 < 10; i++) {
				temp = photoArray[i] + temp;
			}
			int temp2 = 0;
			temp2 = temp / 10;
			resistorArray[resistorIndex] = temp2;
			resistorIndex++;
		}

		if (resistorIndex == 144) {
			resistorIndex = 0;

			std::sort(resistorIndex, resistorIndex + 144);

			int temp3 = 0;
			for (int i = 143; i > 99; i--) {
				temp3 = temp3 + resistorArray[i];
			}
			upperAverage = temp3 / 43;

			int temp4 = 0;
			for (int i = 0; i < 43; i++) {
				temp4 = temp4 + resistorArray[i];
			}
			lowerAverage = temp4 / 43;
		}
	}
}

bool Photoresistor::update(float * res){

}



