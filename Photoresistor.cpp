#include "Photoresistor.h"

Photoresistor::Photoresistor(int a) {
	sensor = new AnalogPort(a);
	isLightOn = 0;
	threshold = 0;
	counter = 0;
	photoIndex = 0;
	resistorIndex = 0;
	upperAverage = 0;
	lowerAverage = 0;

	for (int i = 0; i < 10; i++){
		photoArray[i] = 0;
	}

	for (int j = 0; j < 144; j++){
		resistorArray[j] = 0;
	}
	
}

void Photoresistor::measure() {

	counter++;

	if (counter > 2) {
		counter = 0;

		photoArray[photoIndex] = sensor->read();

		//printf("raw: %d\n",sensor->read());

		photoIndex++;

		if (photoIndex == 9) {
			photoIndex = 0;
			int temp = 0;
			for (int i = 0; i < 10; i++) {
				temp = photoArray[i] + temp;
			}

			temp = temp / 10;

			resistorArray[resistorIndex] = temp;
			resistorIndex++;
		}

		if (resistorIndex == 143) {
			resistorIndex = 0;

			sort(resistorArray, resistorArray + 144);

			int temp3 = 0;
			for (int j = 143; j > 100; j--) {
				temp3 = temp3 + resistorArray[j];
			}

			upperAverage = temp3 / 43;

			int temp4 = 0;
			for (int k = 0; k < 43; k++) {
				temp4 = temp4 + resistorArray[k];
			}
			lowerAverage = temp4 / 43;
		}
	}
}

bool Photoresistor::getLightState(){

	threshold = upperAverage - lowerAverage;
	threshold = threshold * 0.15;
	if(threshold<0){
		threshold=-threshold;
	}

	if (sensor->read() - lowerAverage < threshold && lowerAverage - sensor->read() < threshold){
		return 1;
	}
	else{
		return 0;
	}
}

bool Photoresistor::update(){
	measure();
}
