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
 */
int Room::getTempflow(){
	int tempflow=0;
	switch (room){
	case classRoom:
		/*
		 * if summer (june to august)
		 * 59,9 degrees Fahrenheit
		 *
		 * if spring (march to may)
		 * 37,94 degrees Fahrenheit
		 *
		 * if winter (december to february)
		 *
		 * 21,38 degrees Fahrenheit
		 *
		 * if autnum (september to november)
		 * 41,36 degrees Fahrenheit
		 */
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

