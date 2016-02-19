#include "Heater.h"

Heater::Heater(){
	//initialize heater...
}

bool Heater::update(int t){
	if (t!=0){
		temp=t;
	}
	//check if heater working as should

	return 1;
}

void Heater::setTemp(int t){
	//sets target temperature
	temp=t;
}
