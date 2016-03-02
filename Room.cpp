/*
 * Room.cpp
 *
 *  Created on: 16.2.2016
 *      Author: Olli
 */

#include "Room.h"
#include "LiquidCrystal.h"
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
#define auditoriumPeopleDensity 6
#define computerLabPeopleDensity 2
#define classRoomPeopleDensity 3
#define officePeopleDensity 10
#define airDensity 1.205
#define specificHeat 1005
#define peopleMultiplier 600
#define machineryMultiplier 300
#define windowMultiplier 170
#define recoveryEfficiency 50
#define wallmaterialConductance 0.317
#define heatCapacity 1.01
#define standardHeatingTime 1800 //sec
int mollier[7][10]{
	{0,1,2,3,4,5,6,7,8,9},
	{5,7,8,9,10,11,13,14,15,17},
	{10,12,14,16,18,20,22,24,26,28},
	{15,18,21,24,27,30,33,35,38,41},
	{20,24,28,32,36,40,44,48,52,56},
	{25,30,35,40,45,50,55,60,65,70},
	{30,36,42,48,54,60,66,72,78,84}
};

Room::Room(int floor, float height1, float temp, roomType type, int outer){
	area= floor*1000;
	height= height1*1000;
	space= floor * height1;
	room = type;
	outerWalls= outer;
	 temperature=temp;

}
/*
 * returns space in mm3
 */
int Room::setCubicValues(int floor, int height1){
	if(floor==0){
		floor=area;
	}
	if(height1==0){
		height1=height;
	}
	height= height1*1000;
	area= floor*1000;
	return height * area;

}

void Room::setRoomtype(roomType r){

	room= r;
}
/*
 * returns the desired temperature
 */
float Room::setTemperatureValues(float desiredTemp){

		temperature = desiredTemp+heatloss;
				return temperature;
}


void Room::setindoorTemp(float indtmp){
	indoorTemp=indtmp;
}
void Room::setRecovery(bool reco){
	recovery= reco;
}
void Room::setOuterWalls(int outw){
	outerWalls=outw;
}
/*
 * http://www.engineeringtoolbox.com/design-ventilation-systems-d_121.html
 * If air is used for heating, the needed air flow rate may be expressed as  qh = Hh / (ρ cp (ts - tr))
 * heat transfer coefficient for air is ~0.0257(W/m2K)
 * thermal conductivity of plexiglass is 0.17mass units per 0.19 (or -0.19)meters per 0.2 (or -0.2)seconds at 21,85 celcius (or at 295 Kelvin)
 * For offices with average insulation and lighting, 2/3 occupants and 3/4 personal computers and a photocopier, the following calculations will suffice:
 * Heat load (BTU) = Length (ft.) x Width (ft.) x Height (ft.) x 4
 *
 * Heat load (BTU) = Length (m) x Width (m) x Height (m) x 141
 * For every additional occupant add 500 BTU.
 * function needs to know:
 * heat of air supply ts
 * heat of indoor air tr
 * Specific heat air = standard at 18-25 celcius ~1.005
 * volume of indoor air(heatload) Hh <--this value needs to be tweeked if we want to specify the room type
 * density of air is pretty standard at 18-25 celcius ~1.205
 *
 * Function returns m3/h
 */
float Room::getAirflow(float people,float windows, int machinery){

	//float indoorTemp=21.0, airSupplyTemp=21.0;

	float heatload=(10.7639104*area);//converting squaremeters to squarefeet

	switch (room){
	case classRoom:
		people = area/classRoomPeopleDensity;
		heatload= (heatload+(people*peopleMultiplier)+(windows*windowMultiplier))*0.29307107;//adding the effect from windows people etc. and converting BTU to watt
		airflow = (heatload/(airDensity*(specificHeat*(indoorTemp - radiatorHeat))))*3600;// put it in a airFlow function and turn it to m3/h
		printf("\n airflow= %5.2f heatload= %5.2f",airflow, heatload);
		return airflow;
		break;
	case computerLab:
		people = area/computerLabPeopleDensity;
		heatload= (heatload+(people*peopleMultiplier)+(machinery*machineryMultiplier))*0.29307107;
		airflow = (heatload/(airDensity*(specificHeat*(indoorTemp - radiatorHeat))))*3600;// put it in a airFlow function and turn it to m3/h
		return airflow;
		break;
	case auditorium:
		people = area/auditoriumPeopleDensity;
		heatload= (heatload+(people*peopleMultiplier))*0.29307107;
		airflow = (heatload/(airDensity*(specificHeat*(indoorTemp - radiatorHeat))))*3600;// put it in a airFlow function and turn it to m3/h
		return airflow;
		break;
	case office:
		people = area/officePeopleDensity;
		airflow = (heatload/(airDensity*(specificHeat*(indoorTemp - radiatorHeat))))*3600;
		return airflow;
		break;
	}
	return 0;
}
/*
 * If 10.1 kJ is added to 1 kg air the temperature rise can be calculated as:
 * tB - tA = (10.1 kJ/kg) / (1.01 kJ/kgCelcius)= 10Celcius
 */
float Room::getAirSupplyTemp(float sensorTemp){
	radiatorHeat= (getAirflow()/3600)*(specificHeat/1000)*heatCapacity*(temperature-sensorTemp);
	radiatorHeat-= (outerWallHeatLoss(outerWalls)+getVentHeatLoss(recovery))/1000;//radiatorHeat is in kilowatts
	int ok=0;
	int b = standardHeatingTime;
	do{
	int a = radiatorHeat/((getAirflow()/3600)*b);
	printf("\n radiatorHeat = %5.2f", radiatorHeat);
	ok++;
	b+=300;
	if(a < heaterMAX){
		ok=25;
		radiatorHeat=a;
	}
	}while (ok<22);
	if(ok==22){
		err=false;
	}
	return radiatorHeat;
}
/*
 * http://www.worldweatheronline.com/helsinki-weather-averages/southern-finland/fi.aspx
 */
float Room::getTempDiff(float inside, int month){
	int outside=0;
		outside=monthTempHelsinki[month];
		if(outside < 0){
				differentTemp=inside+outside;
			}
			differentTemp=inside-outside;

	return differentTemp;
}
/*
 * gets how many watts/hour ventilation loses heat
 */
float Room::getVentHeatLoss(bool recovery){

	heatloss=specificHeat*airDensity*(getAirflow()*(differentTemp));
	if(recovery){
		heatloss=(1 - recoveryEfficiency/100)*heatloss;
	}
	return heatloss;
}
/*
 * returns how many watts/hour walls lose heat
 */

float Room::outerWallHeatLoss(int outw){

	outerWalls=outw;
	wallsize= sqrt(area)*height;
	return outw*(wallmaterialConductance*wallsize*differentTemp);

}

bool Room::update(float Tmp, int mon){
getTempDiff(Tmp,mon);
getAirSupplyTemp(Tmp);
return err;
}
