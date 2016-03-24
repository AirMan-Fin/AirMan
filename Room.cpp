/*
 * Room.cpp
 *
 *  Created on: 16.2.2016
 *      Author: Olli
 */

#include "Room.h"
#include <math.h>
#include <cstdio>
/*
 * area is in mm2
 * height is in mm
 * space are in mm3
 * Room type defines what use the room is going to be used
 * enum types are:
 * classRoom=0
 * computerLab=1
 * auditorium=2
 * office=3
 * publicArea=4
 * temperature is the desired temperature
 * humidity is the desired humidity
 * indoorTemp is the temperature indoors
 * by default floor = 40m2, height1=2.5m, temp = 21.00C, roomType=classRoom, outerwalls=0
 * Month= january, people=0, windows=0, machinery=0 and no recovery unit
 */
#define auditoriumPeopleDensity 6//one person per 6m^2
#define computerLabPeopleDensity 2//one perrson per 2m^2
#define classRoomPeopleDensity 3//one person per m^2
#define officePeopleDensity 10//etc.

#define auditoriumMachineryDensity 0//one person per 6m^2
#define computerLabMachineryDensity 2//one perrson per 2m^2
#define classRoomMachineryDensity 0.1//one person per m^2
#define officeMachineryDensity 0.2//etc.

#define classRoomBoost 1.2
#define computerLabBoost 1.5
#define auditoriumBoost 1.5
#define officeBoost 1.1

#define airDensity 1.205 //kg/m^3
#define specificHeat 1.005//kJ(kg/K)
#define peopleMultiplier 80//watts according to Ari Hokkanen but 120 according to http://www.engineeringtoolbox.com/metabolic-heat-persons-d_706.html
#define machineryMultiplier 0//watts only servers etc. affects http://www.engineeringtoolbox.com/heat-gain-equipment-d_1668.html
#define windowMultiplier //watts couldn't figure out how much windows actually produce
#define recoveryEfficiency 0.5 // precent how much recovery unit saves energy
#define wallmaterialConductance 0.317//how much material takes away energy
#define standardHeatingTime 1800 //sec to reach desire temperature
#define optimalHumidity 0.5 //optimal humidity is between 45-55%

int mollier[7][10] {
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 5, 7, 8, 9, 10, 11, 13, 14, 15, 17 },
		{ 10, 12, 14, 16, 18, 20, 22, 24, 26, 28 },
		{ 15, 18, 21, 24, 27, 30, 33, 35, 38, 41 },
		{ 20, 24, 28, 32, 36, 40, 44, 48, 52, 56 },
		{ 25, 30, 35, 40, 45, 50, 55, 60, 65, 70 },
		{ 30, 36, 42, 48, 54, 60, 66, 72, 78, 84 } };

Room::Room(Eeprom *e, int floor, float height1, float temp, roomType type,
		int outer, bool reco) {
	eeprom = e;
	setCubicValues(floor, height1); // sets cubics
	setRoomtype(type); //sets the rooom type classRoom etc.
	setTemperatureValues(temp); //sets the desired temperature
	setRecovery(reco); //is there a recovery unit
	setOuterWalls(outer); //sets the amount of outer walls
	targetTime = standardHeatingTime;
#ifndef testMode
	float tt;
	uint8_t buff[2];
	if (eeprom->readAndCombine(areaMem, 4, &tt, 4)) {
		area = tt;
	}

	if (eeprom->readAndCombine(heightMem, 2, &tt, 1)) {
		height = tt;
	}
	if (eeprom->read(recoveryMem, buff)) {
		recovery = buff[0];
	}
	if (eeprom->read(outerwallMem, buff)) {
		outerWalls = (int) buff[0];
	}
	if (eeprom->read(roomTypeMem, buff)) {
		room = (roomType) buff[0];
	}
#endif
}
bool Room::update(float Tmp, int mon, float humidity1) {
	if (month != mon) { // changes heater min air temperature
		month = mon;
	}
	targetTime=1800;

	setSensorTemp(Tmp);
	getTempDiff(Tmp, mon); //calculates temperature difference between outside and inside
	getHeatLoss(); //calculate energy room produces/loses

	calculateAirflow(); //calculate needed airflow to chance room air (people, space)
	err = getTargetEnergy(); //if we need to increase / decrease temperature, calculate air temperature needed for that, then time to do that with current known airflow


	trimmer();

	printf("Air= %3.4f Temp= %3.1f, Time= %3.1\n ", targetAirflow,
			blowingTemperature, targetTime);

	//getAirSupplyTemp();  //adjust airflow and time to be more user friendly

	return err;
}
/*
 * note:
 * decreasing time increases airflow
 * increasing airflow decreases blowingTemperature
 * decreasing time increases temperature
 */
void Room::trimmer() { //fine tune function
	int ok = 0;
	float trimmerMultiplier=0;
	while (ok < 10) {
		ok++;
		if (blowingTemperature < userHeaterMIN) {
			trimmerMultiplier = ((sensorTemp - blowingTemperature)- (temperature - userHeaterMIN))/ (sensorTemp - blowingTemperature);
			blowingTemperature =userHeaterMIN;//assignment would just result to code visiting trimmer just every other time
			targetAirflow *= trimmerMultiplier;
			targetTime*=trimmerMultiplier;//

		}
		if(blowingTemperature > userHeaterMAX){
				trimmerMultiplier=userHeaterMAX/blowingTemperature;
				blowingTemperature=userHeaterMAX;
				targetAirflow*=trimmerMultiplier;
				targetTime*=trimmerMultiplier;


			}
		if (MAXairflow < targetAirflow) {

			trimmerMultiplier = targetAirflow / MAXairflow;
			targetAirflow = MAXairflow;
			if ((blowingTemperature * trimmerMultiplier) < userHeaterMAX) {
				blowingTemperature *= trimmerMultiplier;
			}
			else{
				targetTime*=trimmerMultiplier;
			}
		}
		if(humidity > optimalHumidity){
			trimmerMultiplier=humidity-optimalHumidity;
			if (MAXairflow < (targetAirflow*trimmerMultiplier)){
				targetAirflow*=(1+trimmerMultiplier);
			}
		}
		if(targetAirflow < MINairflow){
			trimmerMultiplier= MINairflow/targetAirflow;
			targetAirflow*=trimmerMultiplier;
			targetTime*=trimmerMultiplier;
		}
		if(powersave){
			targetAirflow*=0.99;
		}


		/*if(temperature +0,25 >= sensorTemp && sensorTemp <= temperature -0,25){//if you have recieved 100 samples that the air is at same temperature you can lower airflow
		 tMeas++;
		 if(tMeas >= 100){

		 }
		 }
		 else if(temperature +0,25<= sensorTemp && sensorTemp >= temperature -0,25){
		 tMeas=0;
		 }*/
	}
}

/*
 * returns m3/h
 * effect of boost is adde
 * d here
 */
void Room::calculateAirflow() {

	if(!userAirflow){
	//float humidityDifference = humidity-optimalHumidity;//calculate humidity difference
	airflow = space / targetTime; // put it in a airFlow function and turn it to m3/s
	}
	else{
		airflow=userAirflow;

	}

	airflow = airflow * ((humidity / 5) + 1); //calculate the effect of humidity raise airflow if too much humidity etc.

	if (boost > 0) {
		switch (room) {
		case classRoom:
			airflow = airflow * classRoomBoost;
			break;
		case computerLab:
			airflow = airflow * computerLabBoost;
			break;
		case auditorium:
			airflow = airflow * auditoriumBoost;
			break;
		case office:
			airflow = airflow * officeBoost;
			break;
		}
		boost -= 10;
	}
	/*if(airflow < MINairflow){
	 targetTime=targetTime*(1-(airflow*MINairflow));//decrease time
	 }*/
	targetAirflow = airflow;
}
/*
 * note:
 * increasing time decreases temperature and airflow
 *
 */
bool Room::getTargetEnergy() {
	bool ret = 1;
	int ok = 0;
	float ero = temperature - sensorTemp;
	float energy = ero * space * specificHeat;
	float celcius = 0;
	//float energyInRoom = (space*specificHeat*sensorTemp*airDensity);
	//energy= energyInRoom+energy;
	energy = energy - heatTotal;
	int aika = standardHeatingTime;
	//	do{

	celcius = (energy / aika) / (airDensity * targetAirflow * specificHeat);
	celcius = celcius + temperature;		//the energy difference in celcius
	ok++;
	//if(celcius < heaterMAX && celcius> heaterMIN){
	ok = 25;
	blowingTemperature = celcius;
	targetTime = aika;
	//}
	/*} while (ok < 22);
	 if (ok == 22) {
	 ret = 0;
	 }*/
	return ret;

}
/*
 * http://www.worldweatheronline.com/helsinki-weather-averages/southern-finland/fi.aspx
 */
float Room::getTempDiff(float inside, int month) {
	int outside = 0;
	outside = monthTempHelsinki[month];

	differentTemp = inside - outside;

	return differentTemp;
}
/*
 * gets how many watts/hour ventilation loses heat
 */
void Room::getHeatLoss() {

	float wall = 0; // how much energy loss is througth walls
	float vent = 0; // how much energy loss is via exhaust air

	vent = specificHeat * airDensity * airflow;

	if (recovery) {
		vent = (1 - recoveryEfficiency) * vent;
	}

	wallsize = sqrt(area) * height;
	wall = outerWalls * (wallmaterialConductance * wallsize * differentTemp);

	float heatloss = wall + vent;

	float heatload = 0; //how much room produces energy

	heatload = (PeopleDensity * peopleMultiplier); //+ (Windows * windowMultiplier)
	//+ (MachineryDensity * machineryMultiplier); //adding the effect from windows people etc. watts
	heatTotal = heatload - heatloss;

	//starting to change it to celcius
	/*
	 * amount of heat energy q
	 * is equal to it's mass m
	 * multiplied by specificHeat Cg
	 * multiplied by difference in temperature dt
	 * so q=m*Cg*dt
	 * dt=(m*Cg)/q
	 * didn't work
	 *
	 *
	 */
	//heatloss= (heatloss/time)/(airDensity* /* m^3/s*/specificHeat);//heatloss per second
}
void Room::setHumidity(float hum){
	humidity=hum;
}
void Room::setPowerSave(bool b) {
	b=powersave;
	if(boost>0){
		powersave = false;
	}
	if(powersave){
		PeopleDensity = 0;
	}
	else{
		setRoomtype(room);
	}
}

/*
 * returns space in mm3
 */
int Room::setCubicValues(float floor, float height1) {
	if (floor == 0) {
		floor = area;
	}
	if (height1 == 0) {
		height1 = height;
	}
	height = height1;
	area = floor;
	space = area * height;
#ifndef testMode
	eeprom->writeNumber(areaMem, area, 4);
	eeprom->writeNumber(heightMem, height, 2);
#endif
	return space;

}

void Room::setRoomtype(roomType r) {//sets and defines room type and it's properties

	room = r;
#ifndef testMode
	eeprom->writeNumber(roomTypeMem, room, 1);
#endif

	switch (r) {
	case classRoom:
		PeopleDensity = area / classRoomPeopleDensity;
		//MachineryDensity = area / classRoomMachineryDensity;
		Windows = outerWalls;
		break;
	case computerLab:
		PeopleDensity = area / computerLabPeopleDensity;
		//MachineryDensity= area / computerLabMachineryDensity;
		Windows = outerWalls;
		break;
	case auditorium:
		PeopleDensity = area / auditoriumPeopleDensity;
		//MachineryDensity= area / auditoriumMachineryDensity;
		Windows = outerWalls;
		break;
	case office:
		PeopleDensity = area / officePeopleDensity;
		//MachineryDensity= area / officeMachineryDensity;
		Windows = outerWalls;
		break;
	}
}
void Room::setUserAirflow(float user){
	B_userAirflow=1;
	userAirflow=user;
	if(user=0){
		B_userAirflow=0;
	}
}
void Room::setTemperatureValues(float desiredTemp) {//sets the desired temperature value
	temperature = desiredTemp;
}
void Room::setBoost(float hours) {
	boost = hours * 360;
}
void Room::setSensorTemp(float tmp){
	sensorTemp=tmp;
}
void Room::setRecovery(bool reco) {			//do you have recovery unit
	recovery = reco;
#ifndef testMode
	eeprom->writeNumber(recoveryMem, (int) recovery, 1);
#endif
}
void Room::setOuterWalls(float outw) {			//how many outer walls
	outerWalls = outw;
#ifndef testMode
	eeprom->writeNumber(outerwallMem, outerWalls, 1);
#endif
}
float Room::getSpaceValue() {
	return space;
}
float Room::getHeigthValue() {
	return height;
}
roomType Room::getRoomtype() {
	return room;
}
/*
 * returns true if there is an recovery unit
 */
bool Room::getRecovery() {
	return recovery;
}
/*
 * returns te amount of outerwalls
 */
int Room::getOuterWalls() {
	return outerWalls;
}
/*
 * returns the current airflow
 */
float Room::getAirflow(){
	return targetAirflow;
}
/*
 * returns the temperature of incoming air
 */
float Room::getBlowingTemp(){
	return blowingTemperature;
}
float Room::getTemperatureValue(){
return temperature;
}
