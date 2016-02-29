#ifndef ROOM_H
#define ROOM_H

#include "arduino.h"

class Room{
private:
	int area;
	int height;
	int space;

	int people;
	int airVariable;
	int tempVariable;
	float temperature;
	float heatloss=0;
	float airflow;
	int temperate;
	int humidity;
	enum roomType{
		classRoom=0,
		computerLab=1,
		auditorium=2,
		office=3,
		publicArea=4
	};
	roomType room;

	void calculate();// not used
public:
	Room(int,float,float,roomType);
	void update();//not used

	float getAirflow(float,float,float,float,int);
	float getVentHeatLoss(float,float,bool);

	int newCubicValues(int, int);
	int newPersonValues(int);
	int newFurnitureValues(int);
	float newTemperatureValues(float,float,float);
	void save();//not used
	void setHeat();
};

#endif
