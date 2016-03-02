#ifndef INTERFACE_H
#define INTERFACE_H

#include "arduino.h"
#include "LiquidCrystal.h"
#include "Room.h"
#include "MenuHandler.h"
#include "MenuItem.h"
#include "EndMenu.h"
#include "Clock.h"
#include <string>

const char *guide[]={//rows to guide
		"Quick guide:",
		"don't lick",    ////
		"don't eat",
		"and so on..."
};
const int guideLength=4;

class MainMenu: public MenuItem {
private:

public:
	MainMenu(std::string nam) {
		name = nam;
		timer = 0;
	}

	void up() {
		timer = 0;
		MenuItem::up();
	}

	void down() {
		timer = 0;
		MenuItem::down();
	}

	void display() {

		func();
		//printf((mm[place]->name).c_str());
		//printf("\n");
		lcd->print(mm[place]->name);
	}

	void tick() {
		timer++;
		if (timer > 10) {
			timer = 0;
			place = 0;
			MenuItem::ok();
			(*current)->display();
		}
	}
};

class DataMenu: public MenuItem {
private:
	float *temp;
	int * err;
	Clock * clock;
	int value;bool modState;
public:
	DataMenu(std::string nam, Clock *c, float *t, int * er) {
		name = nam;
		clock = c;
		temp = t;
		timer = 0;
		value = 0;
		err = er;
		modState = 0;

	}

	void ok() {
		modState = !modState;
		if (modState) {
			//room->setBoost(value);
		}
	}
	void up() {
		if (!modState) {
			if (timer > 0) {
				//room->setTemperatureValues(room->getTemperatureValues() + 0.5);
			}
			timer = 3;
		}

		else {
			if (value < 6)
				value++;
		}
	}
	void down() {
		if (!modState) {
			if (timer > 0) {
				//room->setTemperatureValues(room->getTemperatureValues() - 0.5);
			}
			timer = 3;
		} else {
			if (value > 0)
				value--;
		}
	}
	void back() {
		if (modState) {
			modState = !modState;
		} else
			MenuItem::back();
	}

	/*
	 * basic data display
	 * temp    time
	 * 23.2    15.42
	 */
	void display() {
		if (modState) {
			lcd->clear();
			lcd->print("Boost mode:");
			lcd->setCursor(0, 1);
			lcd->print("hours: ");
			lcd->print(value);
		} else {
			lcd->clear();
			lcd->print("temp    time  ");
			if (*(err) == 0)
				lcd->print("ok");
			else
				lcd->print("ER");
			lcd->setCursor(0, 1);
			if (timer == 0)
				int len = lcd->print(*temp);
			else {
				lcd->print("-");
				//lcd->print(room->getTemperatureValues());
			}
			lcd->setCursor(5, 1);
			lcd->print("C");
			lcd->setCursor(8, 1);
			if (clock->hour < 10)
				lcd->print("0");
			lcd->print(clock->hour);
			lcd->print(".");
			if (clock->min < 10)
				lcd->print("0");
			lcd->print(clock->min);

			if(*(err)!=0){
				lcd->setCursor(15,1);
				lcd->print(*(err));
			}
		}
	}

	void tick() {
		display();
		if (timer > 0)
			timer--;
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
		name = nam;
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
		name = nam;
		intervall = inter;
	}

	void ok() {
		if (modState) {
			value = temp;
			room->setTemperatureValues(value);
		} else {

		}
		modState = !modState;
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
	RoomSpaceMenu(std::string nam, int t, int min2_ = 0, int max2_ = 100,
			int inter = 1) {
		value = temp = t;
		min = min2_;
		max = max2_;
		name = nam;
		intervall = inter;
		modState2 = 0;
		temp2 = 3;
		value2 = 3;
		max2 = 10;
		min2 = 0;
	}

	void ok() {
		if (modState2 == 1) {
			value = temp;
		} else if (modState2 == 2) {
			value2 = temp2;
			room->setCubicValues(value, value2);
		}
		modState2++;
		if (modState2 == 3)
			modState2 = 0;
	}
	void back() {
		modState = 0;
		modState2 = 0;
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
			lcd->print("cubic: ");
			lcd->print(temp + temp2);
			lcd->print(" m^3");
		}
		if (modState2 == 1) {
			lcd->print("area:");
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

const roomType helpBlock[] = { classRoom, computerLab, auditorium, office

};
class RoomTypeMenu: public EndMenu {

public:
	RoomTypeMenu(std::string nam, int t, int min2 = 0, int max2 = 100,
			int inter = 1) {
		value = temp = t;
		min = min2;
		max = max2;
		name = nam;
		intervall = inter;
	}

	void ok() {
		if (modState) {
			value = temp;
			room->setRoomtype(helpBlock[(int) value]);
		} else {

		}
		modState = !modState;
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
		else if (temp == 2)
			lcd->print("auditorium");
		else if (temp == 3)
			lcd->print("office");
		if (modState)
			lcd->print("}");

	}

};

class RoomWindowsMenu: public EndMenu {

public:
	RoomWindowsMenu(std::string nam, int t, int min2 = 0, int max2 = 100,
			int inter = 1) {
		value = temp = t;
		min = min2;
		max = max2;
		name = nam;
		intervall = inter;
	}

	void ok() {
		if (modState) {
			value = temp;
			room->setOuterWalls(value);
		} else {

		}
		modState = !modState;
	}

};

class RoomHeatRecoverMenu: public EndMenu {

public:
	RoomHeatRecoverMenu(std::string nam, int t, int min2 = 0, int max2 = 100,
			int inter = 1) {
		value = temp = t;
		min = min2;
		max = max2;
		name = nam;
		intervall = inter;
	}

	void ok() {
		if (modState) {
			value = temp;
			room->setRecovery(value);
		} else {

		}
		modState = !modState;
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

class RoomSetupMenu: public EndMenu {
	int state;
	EndMenu *curr[5];
	RoomSpaceMenu *space;
	RoomTypeMenu *type;
	RoomTempMenu *temp;
	RoomWindowsMenu *outer;
	RoomHeatRecoverMenu *reco;

public:
	RoomSetupMenu(std::string nam, RoomSpaceMenu *s, RoomTypeMenu *t,
			RoomTempMenu *te, RoomWindowsMenu *o, RoomHeatRecoverMenu *r) {
		name = nam;
		state = 6;

		curr[0] = space = s;
		curr[1] = space = s;
		curr[2] = type = t;
		curr[3] = temp = te;
		curr[4] = outer = o;
		curr[5] = reco = r;
	}

	void ok() {
		if (state != 6 && state != 0) {
			curr[state]->ok();
		}
		state++;
		if (state > 6) {
			state = 0;
		}
		if (state != 6) {
			curr[state]->ok();
		}
	}

	void back() {
		state = 6;
		EndMenu::back();
	}

	void up() {
		curr[state]->up();
	}

	void down() {
		curr[state]->down();
	}

	void display() {
		if (state == 6) {
			func();
		} else {
			curr[state]->display();
		}
	}
};

class SettingsErrorMenu: public EndMenu {
private:
	bool * errors;
public:
	SettingsErrorMenu(std::string nam, bool * er) {
		name=nam;
		errors=er;
	}

	void display(){
		func();
		if(errors[0])
			lcd->print(1);
		lcd->setCursor(3,1);
		if(errors[1])
			lcd->print(2);
		lcd->setCursor(5,1);
		if(errors[2])
			lcd->print(3);
		lcd->setCursor(7,1);
		if(errors[3])
			lcd->print(4);
		lcd->setCursor(9,1);
		if(errors[4])
			lcd->print(5);
		lcd->setCursor(11,1);
		if(errors[5])
			lcd->print(6);
		lcd->setCursor(13,1);
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
		name = nam;
		clock = c;
		modState2 = 0;

	}

	void ok() {
		modState2++;
		if (modState2 > 5) {
			modState2 = 0;
		}
		if (modState2 == 0) {
			clock->modState = 0;
		} else {
			clock->modState = 1;
		}

	}

	void back() {
		clock->modState = 0;
		modState2 = 0;
		EndMenu::back();
	}

	void up() {
		switch (modState2) {
		case 1:
			if (clock->hour < 24)
				clock->hour++;
			break;
		case 2:
			if (clock->min < 60)
				clock->min++;
			break;
		case 3:
			if (clock->day < 32)
				clock->day++;
			break;
		case 4:
			if (clock->month < 13)
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
			if (clock->hour > 0)
				clock->hour--;
			break;
		case 2:
			if (clock->min > 0)
				clock->min--;
			break;
		case 3:
			if (clock->day > 0)
				clock->day--;
			break;
		case 4:
			if (clock->month > 0)
				clock->month--;
			break;
		case 5:
			if (clock->year > 0)
				clock->year--;
			break;
		}
	}

	void display() {
		func();
		if (modState2 == 1)
			lcd->print("{");
		if (clock->hour < 10)
			lcd->print("0");
		lcd->print(clock->hour);
		lcd->print(".");

		if (modState2 == 2)
			lcd->print("{");
		if (clock->min < 10)
			lcd->print("0");
		lcd->print(clock->min);
		lcd->print(" ");

		if (modState2 == 3)
			lcd->print("{");
		if (clock->day < 10)
			lcd->print("0");
		lcd->print(clock->day);
		lcd->print(".");

		if (modState2 == 4)
			lcd->print("{");
		if (clock->month < 10)
			lcd->print("0");
		lcd->print(clock->month);
		lcd->print(".");

		if (modState2 == 5)
			lcd->print("{");
		lcd->print(clock->year % 100);

		if (clock->modState)
			lcd->print("}");

	}
};

class SettingsGuideMenu: public EndMenu{
public:
	SettingsGuideMenu(std::string nam){
		name=nam;
		value=0;

	}
	void up(){
		if(value<guideLength-1){
			value++;


		}
	}
	void down(){
		if(value>0){
			value--;
		}
	}
	void diplay(){
		lcd->clear();
		lcd->print(guide[(int)value]);
		lcd->setCursor(0,1);
		lcd->print(guide[(int)value+1]);
	}
};

class Interface {
private:
	LiquidCrystal *lcd;bool butPressed[4];
	Room *room;
	Button *button[4];

	MenuHandler *menuHandler;

	MainMenu *main;
	RoomMenu *roomMenu;
	SettingsMenu *settingsMenu;
	DataMenu *dataMenu;

	SettingsClockMenu *settingsClockMenu;
	SettingsErrorMenu *settingsErrorMenu;

	RoomSpaceMenu *roomSpaceMenu; //area, height
	RoomTypeMenu *roomTypeMenu; // room mtype
	RoomTempMenu *roomTempMenu; // room target temperature
	RoomWindowsMenu *roomWindowsMenu; //outer walls
	RoomHeatRecoverMenu *roomHeatRecoverMenu; //Heat recovering unit
	RoomSetupMenu *roomSetupMenu; //Setups all other room settings

	void readButtons();
	void handle(key);

public:

	/*
	 * creates required menu instances
	 * creates lcd and room instances
	 */
	Interface(Room * r, Clock * c, float * temp, int * er, bool * erro, int rs = 8, int en = 9,
			int d4 = 10, int d5 = 11, int d6 = 12, int d7 = 13);
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
