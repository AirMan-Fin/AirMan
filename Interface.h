#ifndef INTERFACE_H
#define INTERFACE_H

#include "arduino.h"
#include "LiquidCrystal.h"
#include "Room.h"

enum key{
	up,
	down,
	enter,
	back
};

class Interface{
private:
	LiquidCrystal *lcd;
	bool butPressed[4];
	Room *room;
	Button button[4];

	void readButtons();
	void handle(key);

public:
	Interface(Room *,int a=8,int b=9,int c=10,int d=11,int e=12,int f=13);
	~Interface();
	void initButtons(int,int,int,int);
	void lcdBegin(int a=16,int b=2);

	void update();



};

#endif
