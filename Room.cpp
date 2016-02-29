/*
 * Room.cpp
 *
 *  Created on: 16.2.2016
 *      Author: Olli
 */

#include "Room.h"
#include "LiquidCrystal.h"
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
 */
#define auditoriumPeopleDensity 6
#define computerLabPeopleDensity 2
#define classRoomPeopleDensity 3
#define officePeopleDensity 10
#define airDensity 1.205
#define spesificHeat 1005
#define peopleMultiplier 600
#define machineryMultiplier 300
#define windowMultiplier 170
#define recoveryEfficiency 50

Room::Room(int floor = 40, float height1=2.5,float temp = 21.00, roomType type=classRoom){
	area= floor*1000;
	height= height1*1000;
	space= floor * height1;
	room = type;
	temperature =temp;
}
/*
 * returns space in mm3
 */
int Room::newCubicValues(int floor=0, int height1=0){
	if(floor=0){
		floor=area;
	}
	if(height1=0){
		height1=height;
	}
	height= height1*1000;
	area= floor*1000;
	return height * area;

}

/*
 * Returns the number of people
 */
int Room::newPersonValues(int newGuy){
	people =newGuy;
	return people;
}
/*
 * returns the desired temperature
 */
float Room::newTemperatureValues(float desiredTemp=21.0,float insideTemp=21.0,float outsideTemp=15.0){

		temperature = desiredTemp+getVentHeatLoss(insideTemp,outsideTemp,0);
				return temperature;

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
float Room::getAirflow(float indoorTemp=21.0, float airSupplyTemp=21.0,
						float people=0,float windows=0, int machinery=0){

	float heatload=(10.7639104*area);//converting squaremeters to squarefeet

	switch (room){
	case classRoom:
		people = area/classRoomPeopleDensity;
		heatload= (heatload+(people*peopleMultiplier)+(windows*windowMultiplier))*0.29307107;//adding the effect from windows people etc. and converting BTU to watt
		airflow = (heatload/(airDensity*(spesificHeat*(indoorTemp - airSupplyTemp))))*3600;// put it in a airFlow function and turn it to m3/h
		return airflow;
		break;
	case computerLab:
		people = area/computerLabPeopleDensity;
		heatload= (heatload+(people*peopleMultiplier)+(machinery*machineryMultiplier))*0.29307107;
		airflow = (heatload/(airDensity*(spesificHeat*(indoorTemp - airSupplyTemp))))*3600;// put it in a airFlow function and turn it to m3/h
		return airflow;
		break;
	case auditorium:
		people = area/auditoriumPeopleDensity;
		heatload= (heatload+(people*peopleMultiplier))*0.29307107;
		airflow = (heatload/(airDensity*(spesificHeat*(indoorTemp - airSupplyTemp))))*3600;// put it in a airFlow function and turn it to m3/h
		return airflow;
		break;
	case office:
		people = area/officePeopleDensity;
		airflow = (heatload/(airDensity*(spesificHeat*(indoorTemp - airSupplyTemp))))*3600;
		return airflow;
		break;
	}
}

float Room::getVentHeatLoss(float insideTemp=21.0, float outsideTemp=15.0,bool recovery=0){

	heatloss=spesificHeat*airDensity*(getAirflow()*(insideTemp-outsideTemp));
	if(recovery){
		heatloss=(1 - recoveryEfficiency/100)*heatloss;
	}
	return heatloss;
}

void Room::update(){
newTemperatureValues();
getAirflow(temperature);
}
