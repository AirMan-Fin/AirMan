#ifndef ROOM_H
#define ROOM_H

#include "arduino.h"

class Room{
private:
	int cubic;
	int height;
	int space;

	int airVariable;
	int tempVariable;
	int people;
	int furniture;

	int temperate;
	int humidity;

	void calculate();
public:
	Room(int,int);
	void update(int, int);

	int getAirflow();
	int getTempflow();

	int newCubicValues(int sp=space, int he= height);
	int newPersonValues(int);
	int newFurnitureValues(int);
};

#endif
