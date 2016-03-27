#ifndef INTERFACE_H
#define INTERFACE_H

#include "arduino.h"
#include "LiquidCrystal.h"
#include "Room.h"
#include "MenuHandler.h"
#include "MenuItem.h"
#include "EndMenu.h"
#include "Fan.h"
#include "Clock.h"
#include <string>
#include <cstdio>

void drawArrow(LiquidCrystal &lcd, int object, int column, int row);

class MainMenu: public MenuItem {
private:

public:
	MainMenu(Room *r, std::string nam) {
		room = r;
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
		if (isEnd) {
			drawArrow(*lcd, 1, 15, 0);
		}
		if (isStart) {
			drawArrow(*lcd, 2, 15, 1);
		}
	}

	void tick() {
		timer++;
		if (timer > 100) {
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
	float *humi;bool * err;
	Clock * clock;
	int value;bool modState;
	uint8_t timer2;
	uint8_t timer3;

public:
	DataMenu(Room *r, std::string nam, Clock *c, float *t, float *h,
	bool * er) {
		room = r;
		name = nam;
		clock = c;
		temp = t;
		humi = h;
		timer = 0;
		value = 0;
		err = er;
		timer2 = 0;
		timer3 = 0;
		modState = 0;
	}

	void ok() {
		if (timer3 == 0) {
			modState = !modState;
			timer3 = 50;
			if (modState) {
				room->setBoost(value);
				value = 0;
				timer3 = 10;
			}
		}
	}
	void up() {
		if (!modState) {
			if (timer > 0) {
				if (room->getTemperatureValue() < 28)
					room->setTemperatureValues(
							room->getTemperatureValue() + 0.5);
			}
			timer = 30;
		} else {
			if (value < 6)
				value++;
		}
	}
	void down() {
		if (!modState) {
			if (timer > 0) {
				if (room->getTemperatureValue() > 17)
					room->setTemperatureValues(
							room->getTemperatureValue() - 0.5);
			}
			timer = 30;
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
			int a = *temp;

			if (timer == 0)   ///////////temperature printing
				lcd->print((*temp), 1);
			else {
				lcd->print(":");
				lcd->print(room->getTemperatureValue(), 1);
				//printf("%3.2f\n",room->getTemperatureValue());
			}
			lcd->setCursor(5, 0);
			lcd->print("C");

			lcd->setCursor(8, 0); ////////clock printing

			if (clock->hour < 10)
				lcd->print("0");
			lcd->print(clock->hour);
			lcd->print(".");
			if (clock->min < 10)
				lcd->print("0");
			lcd->print(clock->min);

			lcd->setCursor(0, 1); ////////humidity printing
			lcd->print((*humi) * 100, 1);
			lcd->setCursor(5, 1);
			lcd->print("%");

			///////////////////////7 ///// error printing
			bool noErrors = 1;
			for (int a = 0; a < 8; a++) {
				if (err[a]) {
					lcd->setCursor(8, 1);
					lcd->print("ERROR ");
					noErrors = 0;
					lcd->print(a);
				}
			}
			if (noErrors) {
				lcd->setCursor(8, 1);
				lcd->print("ok");
			}
		}
	}

	void tick() {
		timer2++;
		if (timer3 > 0)
			timer3--;
		if (timer3 == 0)
			modState = 0;

		if (timer2 >= 50) {
			timer2 = 0;
			display();
		}

		if (timer > 0)
			timer--;
	}

};

class SettingsMenu: public MenuItem {
private:

public:
	SettingsMenu(Room *r, std::string nam) {
		room = r;
		name = nam;
	}

	void display() {
		func();
		lcd->print(mm[place]->name);
		if (isEnd) {
			drawArrow(*lcd, 1, 15, 0);
		}
		if (isStart) {
			drawArrow(*lcd, 2, 15, 1);
		}
	}
};

class RoomMenu: public MenuItem {
private:

public:
	RoomMenu(Room *r, std::string nam) {
		room = r;
		name = nam;
	}

	void display() {
		func();
		lcd->print(mm[place]->name);
		if (isEnd) {
			drawArrow(*lcd, 1, 15, 0);
		}
		if (isStart) {
			drawArrow(*lcd, 2, 15, 1);
		}
	}

};

class RoomTempMenu: public EndMenu {

public:
	RoomTempMenu(Room *r, std::string nam, float t, float min2 = 0, float max2 =
			100, float inter = 1) {
		room = r;
		value = temp = room->getTemperatureValue();
		min = min2;
		max = max2;
		name = nam;
		intervall = inter;
	}

	void enterMenu() {
		value = temp = room->getTemperatureValue();
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
	RoomSpaceMenu(Room *r, std::string nam, int t, int min2_ = 0, int max2_ =
			100, int inter = 1) {
		room = r;
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

	void enterMenu() {
		value = temp = room->getAreaValue();
		value2 = temp2 = room->getHeigthValue();
		modState2 = 0;
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
			lcd->print(room->getSpaceValue());
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
	RoomTypeMenu(Room *r, std::string nam, int t, int min2 = 0, int max2 = 100,
			int inter = 1) {
		room = r;
		min = min2;
		max = max2;
		name = nam;
		intervall = inter;
		value = temp = room->getRoomtype();
	}

	void enterMenu() {
		value = temp = room->getRoomtype();
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
	RoomWindowsMenu(Room *r, std::string nam, int t, int min2 = 0, int max2 =
			100, int inter = 1) {
		room = r;
		min = min2;
		max = max2;
		name = nam;
		intervall = inter;
		value = temp = room->getOuterWalls();
	}

	void enterMenu() {
		value = temp = room->getOuterWalls();
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
	RoomHeatRecoverMenu(Room *r, std::string nam, int t, int min2 = 0,
			int max2 = 100, int inter = 1) {
		room = r;
		min = min2;
		max = max2;
		name = nam;
		intervall = inter;
		value = temp = room->getRecovery();
	}

	void enterMenu() {
		value = temp = room->getRecovery();
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

class RoomDefaultMenu: public EndMenu {

public:
	RoomDefaultMenu(Room *r, std::string nam, int t, int min2 = 0, int max2 =
			100, int inter = 1) {
		room = r;
		value = temp = t;
		min = min2;
		max = max2;
		name = nam;
		intervall = inter;
	}

	void enterMenu() {
		value = temp = room->getUserBlow();

	}

	void ok() {
		if (modState) {
			value = temp;
			room->setUserAirflow(value);
		} else {

		}
		modState = !modState;
	}
};

class RoomPipeAreaMenu: public EndMenu {
	Fan * fan;

public:
	RoomPipeAreaMenu(Room *r, std::string nam, Fan * f, float min2 = 0,
			float max2 = 2.5, float inter = 0.05) {
		room = r;
		value = temp = fan->getPipeArea();
		min = min2;
		max = max2;
		name = nam;
		intervall = inter;
		fan = f;
	}

	void enterMenu() {
		value = temp = fan->getPipeArea();
	}
	void ok() {
		if (modState) {
			value = temp;
			fan->setPipeArea(value);
		} else {

		}
		modState = !modState;
	}
	void display() {
		func();
		lcd->print(text);
		lcd->print(":");
		if (modState)
			lcd->print("[");
		else
			lcd->print(" ");
		lcd->print(temp, 2);
		if (modState)
			lcd->print("]");
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
	RoomSetupMenu(Room *rr, std::string nam, RoomSpaceMenu *s, RoomTypeMenu *t,
			RoomTempMenu *te, RoomWindowsMenu *o, RoomHeatRecoverMenu *r) {
		room = rr;
		name = nam;
		state = 6;

		curr[0] = space = s;
		curr[1] = space = s;
		curr[2] = type = t;
		curr[3] = temp = te;
		curr[4] = outer = o;
		curr[5] = reco = r;
	}

	void enterMenu() {
		timer = 0;
		state = 6;
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
			if (state != 1)
				curr[state]->enterMenu();
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
			lcd->setCursor(0, 1);
			lcd->print("ok->setup room");
		} else {
			curr[state]->display();
		}
	}
};

class SettingsErrorMenu: public EndMenu {
private:
	bool * errors;
public:
	SettingsErrorMenu(Room *r, std::string nam, bool * er) {
		room = r;
		name = nam;
		errors = er;
	}

	void display() {
		func();
		bool jj = 0;
		if (errors[0]) {
			lcd->print(0);
			jj = 1;
		}
		lcd->setCursor(3, 1);
		if (errors[1]) {
			lcd->print(1);
			jj = 1;
		}
		lcd->setCursor(5, 1);
		if (errors[2]) {
			jj = 1;
			lcd->print(2);
		}
		lcd->setCursor(7, 1);
		if (errors[3]) {
			lcd->print(3);
			jj = 1;
		}
		lcd->setCursor(9, 1);
		if (errors[4]) {
			lcd->print(4);
			jj = 1;
		}
		lcd->setCursor(11, 1);
		if (errors[5]) {
			lcd->print(5);
			jj = 1;
		}
		lcd->setCursor(13, 1);

		if (!jj) {
			lcd->print("No errors");
		}
	}

};

class SettingsModbusMenu: public EndMenu {
private:
	int8_t * pt;
public:
	SettingsModbusMenu(Room *r, std::string nam, int8_t * nn) {
		room = r;
		name = nam;
		pt = nn;
	}

	void ok() {
		if (modState) {
			(*pt) = 3;
		}
		modState = !modState;
	}

	void display() {
		func();
		if (modState) {
			lcd->print("Reset modbus?");
		} else {
			lcd->print("Press ok to reset");
		}

	}

};

class SettingsDisableMenu: public EndMenu {
private:

	bool * pt;
public:
	SettingsDisableMenu(Room *r, std::string nam, bool * nn) {
		room = r;
		name = nam;
		pt = nn;
	}

	void ok() {
		if (modState) {
			(*pt) = !(*pt);
		}
		modState = !modState;
	}

	void display() {
		func();
		if (modState) {
			if (*pt) {
				lcd->print("Enable");
			} else
				lcd->print("Disable");
			lcd->print(" pow. save?");
		} else {
			lcd->print("Ok -> ");
			if (*pt) {
				lcd->print("enable");
			} else
				lcd->print("disable");
		}

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
	SettingsClockMenu(Room *r, std::string nam, Clock * c) {
		room = r;
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
			//lcd->print("{");
			lcd->write(126);
		if (clock->hour < 10)
			lcd->print("0");
		lcd->print(clock->hour);
		lcd->print(".");

		if (modState2 == 2)
			//lcd->print("{");
			lcd->write(126);
		if (clock->min < 10)
			lcd->print("0");
		lcd->print(clock->min);
		lcd->print(" ");

		if (modState2 == 3)
			//lcd->print("{");
			lcd->write(126);
		if (clock->day < 10)
			lcd->print("0");
		lcd->print(clock->day);
		lcd->print(".");

		if (modState2 == 4)
			lcd->write(126);
		if (clock->month < 10)
			lcd->print("0");
		lcd->print(clock->month);
		lcd->print(".");

		if (modState2 == 5)
			lcd->write(126);
		lcd->print(clock->year % 100);

	}
};

class SettingsResetMenu: public EndMenu {
private:
	Room * room;
	Fan * fan;
	bool reseted;
public:
	SettingsResetMenu(Room *r, Fan * f, std::string nam) {
		room = r;
		fan = f;
		name = nam;
		value = 0;
		reseted=0;
	}
	void enterMenu() {
		modState = 0;
		reseted=0;
	}
	void ok() {
		if (!reseted) {
			if (modState) {
				room->reset(1);
				fan->reset(1);
				reseted=1;
			}
			modState = !modState;
		}
	}
	void up() {

	}
	void down() {

	}
	void display() {
		func();
		if (reseted) {
			lcd->print("All restored");
		} else {
			if (!modState) {
				lcd->print("Restore settings?");
			} else {
				lcd->print("Ok -> reset");
			}
		}
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
	SettingsModbusMenu *settingsModbusMenu;
	SettingsDisableMenu *settingsDisableMenu;
	SettingsResetMenu *settingsResetMenu;

	RoomSpaceMenu *roomSpaceMenu; //area, height
	RoomTypeMenu *roomTypeMenu; // room mtype
	RoomTempMenu *roomTempMenu; // room target temperature
	RoomWindowsMenu *roomWindowsMenu; //outer walls
	RoomHeatRecoverMenu *roomHeatRecoverMenu; //Heat recovering unit
	RoomSetupMenu *roomSetupMenu; //Setups all other room settings
	RoomDefaultMenu *roomDefaultMenu; //sets default airflow
	RoomPipeAreaMenu *roomPipeAreaMenu; //sets default airflows

	void readButtons();
	void handle(key);

public:

	/*
	 * creates required menu instances
	 * creates lcd and room instances
	 */
	Interface(Room * r, Clock * c, float * temp, float *humi, Fan *fan,
	bool * er, bool *dis, int8_t * modbus, int rs = 8, int en = 9, int d4 = 10,
			int d5 = 11, int d6 = 12, int d7 = 13);
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
