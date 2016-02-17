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
Room::Room(int floor = 400000, int height1=25000,float temp = 21.00, roomType type=classRoom){
	area= floor;
	height= height1;
	space= floor * height1;
	room = type;
	temperature =temp;
}
/*
 * returns Cubics in mm3
 * returns 0 if no values were changed
 */
int Room::newCubicValues(int floor = cubic, int height1=height){
	if(height != height1 || floor!= area){
	height= height1;
	area= floor;
	return height * area;
	}
	return 0;
}

/*
 * Returns the number of people
 * returns 0 if no values were changed
 */
int Room::newPersonValues(int newGuy){
	if(people != newGuy){
	people =newGuy;
	return people;
	}
	return 0;
}
/*
 * Returns the number of furniture
 * returns 0 if no values were changed
 */
int Room::newFurnitureValues(int newFurniture){
	if(newFurniture != furniture){
		furniture = newFurniture;
		return furniture;
	}
	return 0;
}
/*
 * returns the desired temperature
 * returns 0 if temperature was not altered
 */
float Room::newTemperatureValues(float desiredTemp){

	if(temperature != desiredTemp){
		temperature = desiredTemp;
				return temperature;
	}
	return 0;
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
int Room::getTempflow(float indoorTemp, float airSupplyTemp, float people=0, float windows=0){
	float tempflow=0;
	float heatload=(10.7639104*area);//converting squaremeters to squarefeet

	switch (room){
	case classRoom:
		heatload= (heatload+people+windows)*0.29307107;//adding the effect from windows people etc. and converting BTU to watt
		tempflow = (heatload/(1.205*(1.005(indoorTemp - airSupplyTemp))));
		break;
	case computerLab:

		break;
	case auditorium:

		break;
	case office:

		break;
	case publicArea:

		break;
	}
}
/*
 * http://www.engineeringtoolbox.com/indoor-temperature-humidity-d_114.html
 * Carbon dioxide (CO2) concentration in "clean" air is 575 mg/m3.
 * Huge concentrations can cause headaches and the concentration should be below 9000 mg/m3.
 * sitting person emits 20cm3 of Carbon dioxide per hour
 */
int Room::getAirflow(){
	switch (room){
		case classRoom:

			break;
		case computerLab:

			break;
		case auditorium:

			break;
		case office:

			break;
		case publicArea:

			break;
	}
	return 0;
}

