

#include "Fan.h"

Fan::Fan(Millis *m, Eeprom * ee, int fanspeed) {
	for (int a = 0; a < 100; a++) {
		calibration[a]=0;
	}
	autoMode = 1;

	node = new ModbusMaster(m, 2); // Create modbus object that connects to slave id 2
	node->begin(9600);

	node->writeSingleRegister(0, 0x0406);
	delay(1000);
	node->writeSingleRegister(0, 0x047F);
	delay(1000);

	frequency = AUTOMODEDEFAULTSPEED;

	eeprom=ee;
	uint8_t buff[2];
	eeprom->read(pipeAreaMem,buff,1);
	setPipeArea(0.2*0.01f);
}

void Fan::update(float pres) {

	currentAirSpeed = getAirVelocity(pres);
	if (!(currentAirSpeed - targetAirSpeed < AIRSPEEDACCURACY
			&& targetAirSpeed - currentAirSpeed < AIRSPEEDACCURACY)) {
		if (currentAirSpeed < targetAirSpeed) {
			frequency += 0.1;
		} else {
			frequency -= 0.1;
		}
	} else {
		calibration[((int) frequency) / 5] = (uint8_t) (currentAirSpeed * 100);
	}

	setFrequency(frequency);
}

bool Fan::setFrequency(float freq) {
	bool ok = 0;
	uint8_t result = node->writeSingleRegister(1, (uint16_t)(freq*400));
	if (result == node->ku8MBSuccess) {
		ok = 1;
	}
	return ok;
}

int Fan::setAirFlow(float flow, float pres) {
	//printf("setA: %3.2f, %3.2f \n",flow,pres);
	bool ret = 0;
	if (autoMode) {
		currentAirSpeed = getAirVelocity(pres);
		targetAirSpeed = flow / area;
		if (targetAirSpeed > MAXAIRSPEED) {
			ret += 10;
			targetAirSpeed = MAXAIRSPEED;
		}
	} else {
		frequency = calibration[(int)((targetAirSpeed/MAXAIRSPEED)*100)];
	}
	//printf("%d\n", targetAirSpeed * area);

	if (!setFrequency((uint16_t) frequency)) {
		ret += 1;
	}
	return ret;
}

/*
 * function to calculate airvelocity
 * params: pressure
 * returns velocity
 */
float Fan::getAirVelocity(float pressure) {
	return ((pressure * area) / 1.205);

}

void Fan::setFlowPoint(int point, int fl) {
	if (point == 0) {
		maxFlow = fl;
		flowLimit = MAXAIRFLOW / (float) fl;
	} else {
		calibration[point] = fl;
	}
}

void Fan::setAutoMode(bool mo) {
	autoMode = mo;
}


void Fan::setPipeArea(float ar){
	area=ar;
	uint8_t buff[2];
	buff[0]=(uint8_t)area*100;
	eeprom->write(pipeAreaMem, buff, 1);
}

float Fan::getPipeArea(){
	return area;
}

void Fan::reset(bool b){
	if(b){
		setPipeArea(defaultPipeArea);
	}
}

