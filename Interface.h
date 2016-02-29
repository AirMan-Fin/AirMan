#ifndef INTERFACE_H
#define INTERFACE_H

#include "arduino.h"
#include "LiquidCrystal.h"
#include "Room.h"
#include "MenuHandler.h"
#include "MenuItem.h"
#include "EndMenu.h"
#include "Clock.h"

class MainMenu: public MenuItem {
private:
	int timer;
public:
	MainMenu(std::string nam) {
		name =  nam;
		timer=0;
	}


	void up(){
		timer=0;
		MenuItem::up();
	}

	void down(){
		timer=0;
		MenuItem::down();
	}

	void display() {

		func();
		//printf((mm[place]->name).c_str());
		//printf("\n");
		lcd->print(mm[place]->name);
	}

	void tick(){
		timer++;
		if (timer>10){
			timer=0;
			place=0;
			MenuItem::ok();
		}
	}
};

class DataMenu: public MenuItem {
private:
	int *temp;
	Clock * clock;
public:
	DataMenu(std::string nam, Clock *c) {
		name =  nam;
		clock = c;
	}

	void ok() {

	}
	void up() {

	}
	void down() {

	}
	void back() {
		MenuItem::back();
	}

	/*
	 * basic data display
	 * temp    time
	 * 23.2    15.42
	 */
	void display() {
		lcd->clear();
		lcd->print("temp    time");
		lcd->setCursor(0, 1);
		std::string str = toStr(*temp);
		lcd->print(str);
		for (unsigned int a = 0; a < (8 - str.length()); a++) {
			lcd->write(' ');
		}
		if (clock->hour < 10)
			lcd->print("0");
		lcd->print(clock->hour);
		lcd->print(".");
		if (clock->min < 10)
			lcd->print("0");
		lcd->print(clock->min);

	}

};

class SettingsMenu: public MenuItem {
private:

public:
	SettingsMenu(std::string nam) {
		name = nam;
	}

	void display() {
		func();
		lcd->print(mm[place]->name);
	}
};

class RoomMenu: public MenuItem {
private:

public:
	RoomMenu(std::string nam) {
		name =  nam;
	}

	void display() {
		func();
		lcd->print(mm[place]->name);
	}

};


class RoomTempMenu: public EndMenu {


public:
	RoomTempMenu(std::string nam, float t, float min2 = 0, float max2 = 100,
			float inter = 1) {
		value = temp = t;
		min = min2;
		max = max2;
		name =  nam;
		intervall = inter;
	}

};

class RoomSpaceMenu: public EndMenu {
private:
	int modState2;
	float temp2;
	float value2;
	float max2;
	float min2;
public:
	RoomSpaceMenu(std::string nam, int t, int min2_ = 0, int max2_ = 100, int inter =
			1) {
		value = temp = t;
		min = min2_;
		max = max2_;
		name =  nam;
		intervall = inter;
		modState2=0;
		temp2=3;
		value2=3;
		max2=10;
		min2=0;
	}

	void ok() {
		if (modState2 == 1) {
			value = temp;
		} else if (modState2 == 2) {
			value2 = temp2;
		}
		modState2++;
		if (modState2 == 3)
			modState2 = 0;
	}
	void back() {
		modState = 0;
		temp = value;
		temp2 = value2;
		MenuItem::back();
	}
	void up() {
		if (modState2 == 1) {
			if (temp < max)
				temp += intervall;
		}
		if (modState2 == 2) {
			if (temp2 < max2)
				temp2 += 0.3;
		}
	}
	void down() {
		if (modState2 == 1) {
			if (temp > min)
				temp--;
		}
		if (modState2 == 2) {
			if (temp > min2)
				temp2 -= 0.3;
		}
	}

	void display() {

		func();
		if (modState2 == 0) {
			lcd->print("cubic:");
		}
		if (modState2 == 1) {
			lcd->print("space:");
		}
		if (modState2 == 2) {
			lcd->print("height:");
		}

		if (modState2 != 0)
			lcd->print("{");

		if (modState2 == 0) {
			lcd->print(toStr(temp * temp2).c_str());
		}
		if (modState2 == 1) {
			lcd->print(toStr(temp).c_str());
		}
		if (modState2 == 2) {
			lcd->print(toStr(temp2).c_str());
		}
		if (modState2 != 0)
			lcd->print("}");

	}

};

class RoomTypeMenu: public EndMenu {

public:
	RoomTypeMenu(std::string nam, int t, int min2 = 0, int max2 = 100,
			int inter = 1) {
		value = temp = t;
		min = min2;
		max = max2;
		name =  nam;
		intervall = inter;
	}

	void display() {

		func();
		lcd->print(text);
		lcd->print(":");
		if (modState)
			lcd->print("{");
		else
			lcd->print(" ");

		if (temp == 0)
			lcd->print("classroom");
		else if (temp == 1)
			lcd->print("it lab");
		else if (temp == 3)
			lcd->print("auditorium");
		else if (temp == 4)
			lcd->print("office");
		if (modState)
			lcd->print("}");

	}

};

class RoomWindowsMenu: public EndMenu {

public:
	RoomWindowsMenu(std::string nam, int t, int min2 = 0, int max2 = 100, int inter =
			1) {
		value = temp = t;
		min = min2;
		max = max2;
		name =  nam;
		intervall = inter;
	}

};

class RoomHeatRecoverMenu: public EndMenu {

public:
	RoomHeatRecoverMenu(std::string nam, int t, int min2 = 0, int max2 = 100,
			int inter = 1) {
		value = temp = t;
		min = min2;
		max = max2;
		name =  nam;
		intervall = inter;
	}

	void display() {

		func();
		lcd->print(text);
		lcd->print(":");
		if (modState)
			lcd->print("{");
		else
			lcd->print(" ");

		if (temp)
			lcd->print("yes");
		else
			lcd->print("no");
		if (modState)
			lcd->print("}");

	}

};


class SettingsBrigthnessMenu: public EndMenu {

public:
	SettingsBrigthnessMenu(std::string nam, int t, int min2 = 0, int max2 = 100,
			int inter = 1) {
		value = temp = t;
		min = min2;
		max = max2;
		name =  nam;
		intervall = inter;
	}

};

class SettingsClockMenu: public EndMenu {
private:
	Clock *clock;
	int modState2;

	int tempMin;
	int tempHour;
	int tempDay;
	int tempMonth;
	int tempYear;
public:
	SettingsClockMenu(std::string nam, Clock * c) {
		name =  nam;
		clock = c;
		modState2=0;

	}

	void ok() {
		if (modState2 == 0) {
			clock->modState = 0;
		} else {
			clock->modState = 1;
		}
		modState2++;
		if (modState2 > 5) {
			modState2 = 0;
		}
	}

	void up() {
		switch (modState2) {
		case 1:
			clock->hour++;
			break;
		case 2:
			clock->min++;
			break;
		case 3:
			clock->day++;
			break;
		case 4:
			clock->month++;
			break;
		case 5:
			clock->year++;
			break;
		}
	}

	void down() {
		switch (modState2) {
		case 1:
			clock->hour--;
			break;
		case 2:
			clock->min--;
			break;
		case 3:
			clock->day--;
			break;
		case 4:
			clock->month--;
			break;
		case 5:
			clock->year--;
			break;
		}
	}

	void display() {
		func();
		if (clock->modState)
			lcd->print("{");

		if (clock->hour < 10)
			lcd->print("0");
		lcd->print(clock->hour);
		lcd->print(".");
		if (clock->min < 10)
			lcd->print("0");
		lcd->print(clock->min);

		if (clock->day < 10)
			lcd->print("0");
		lcd->print(clock->day);
		lcd->print(".");
		if (clock->month < 10)
			lcd->print("0");
		lcd->print(clock->month);
		lcd->print(".");
		lcd->print(clock->year);
		if (clock->modState)
			lcd->print("}");

	}
};


class Interface {
private:
	LiquidCrystal *lcd;
	bool butPressed[4];
	Room *room;
	Button *button[4];

	MenuHandler *menuHandler;

	MainMenu *main;
	RoomMenu *roomMenu;
	SettingsMenu *settingsMenu;
	DataMenu *dataMenu;

	SettingsClockMenu *settingsClockMenu;
	SettingsBrigthnessMenu *settingsBrightnessMenu;

	RoomSpaceMenu *roomSpaceMenu; //area, height
	RoomTypeMenu *roomTypeMenu; // room mtype
	RoomTempMenu *roomTempMenu; // room target temperature
	RoomWindowsMenu *roomWindowsMenu; //outer walls
	RoomHeatRecoverMenu *roomHeatRecoverMenu; //Heat recovering unit

	void readButtons();
	void handle(key);

public:

	/*
	 * creates required menu instances
	 * creates lcd and room instances
	 */
	Interface(Clock * c, int rs = 8, int en = 9, int d4 = 10, int d5 = 11,
			int d6 = 12, int d7 = 13);
	~Interface();

	/*
	 * initializes buttons
	 */
	void initButtons(int, int, int, int);

	/*
	 * begins lcd
	 */
	void lcdBegin(int a = 16, int b = 2);

	/*
	 * update function, one to be called regularly from main
	 */
	void update();

	/*
	 * timer for menus
	 */
	void tick();
};

#endif
