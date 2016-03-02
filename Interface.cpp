#include "Interface.h"

void Interface::readButtons() {
	for (int a = 0; a < 4; a++) {
		if (button[a]->read()) {
			butPressed[a] = 1;
		}
	}
}
void Interface::handle(key k) {
	menuHandler->handle(k);
	menuHandler->display();
}

Interface::Interface(Room * r, Clock *c, float * temp, int* er, bool * erro, int rs, int en, int d4, int d5, int d6, int d7) {

	room = r;

	butPressed[0]=0;
	butPressed[1]=0;
	butPressed[2]=0;
	butPressed[3]=1;
	lcd = new LiquidCrystal(rs, en, d4, d5, d6, d7);
	//room = r;

	menuHandler = new MenuHandler();

	/*
	 * creating all required menus
	 */
	main = new MainMenu("Main Menu");
	settingsMenu = new SettingsMenu("Settings");
	roomMenu = new RoomMenu("Room Setting");
	dataMenu = new DataMenu("Data", c, temp, er);

	settingsClockMenu = new SettingsClockMenu("Clock", c);
	settingsErrorMenu = new SettingsErrorMenu("Error messages:", erro);

	roomSpaceMenu = new RoomSpaceMenu("Space", 10, 0, 99999);
	roomTypeMenu = new RoomTypeMenu("Type", 0, 0, 3);
	roomTempMenu = new RoomTempMenu("t.Temp", 20, 16, 28, 0.5);
	roomHeatRecoverMenu = new RoomHeatRecoverMenu("Heat Recover", 0, 0, 1);
	roomWindowsMenu = new RoomWindowsMenu("Outer walls", 1, 0, 4);
	roomSetupMenu = new RoomSetupMenu("Setup", roomSpaceMenu, roomTypeMenu, roomTempMenu, roomWindowsMenu, roomHeatRecoverMenu);

	/*
	 * initializing all menus
	 */
	main->init(menuHandler->getpCurrent(), menuHandler->getpVector(), lcd, room);
	settingsMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
			lcd, room);
	roomMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(), lcd, room);
	dataMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(), lcd, room);

	roomSpaceMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
			lcd, room);
	roomTypeMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
			lcd, room);
	roomHeatRecoverMenu->init(menuHandler->getpCurrent(),
			menuHandler->getpVector(), lcd, room);
	roomWindowsMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
			lcd, room);
	roomTempMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
			lcd, room);
	roomSetupMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
				lcd, room);

	settingsClockMenu->init(menuHandler->getpCurrent(),
			menuHandler->getpVector(), lcd, room);
	settingsErrorMenu->init(menuHandler->getpCurrent(),
			menuHandler->getpVector(), lcd, room);

	/*
	 * add starting menu
	 */
	menuHandler->addMenu(*main);

	/*
	 * connect menus to submenus
	 */
	main->addMenuitem(dataMenu);
	main->addMenuitem(roomMenu);
	main->addMenuitem(settingsMenu);

	settingsMenu->addMenuitem(settingsClockMenu);
	settingsMenu->addMenuitem(settingsErrorMenu);

	roomMenu->addMenuitem(roomSetupMenu);
	roomMenu->addMenuitem(roomSpaceMenu);
	roomMenu->addMenuitem(roomTempMenu);
	roomMenu->addMenuitem(roomTypeMenu);
	roomMenu->addMenuitem(roomHeatRecoverMenu);
	roomMenu->addMenuitem(roomWindowsMenu);

	/*
	 * first display
	 */
	menuHandler->display();
}

Interface::~Interface() {
	//delete room;
	//delete lcd;
}
void Interface::initButtons(int b1, int b2, int b3, int b4) {
	int temp[4] = { b1, b2, b3, b4 };
	for (int a = 0; a < 4; a++) {
		button[a] = new Button(temp[a]);
	}
}
void Interface::lcdBegin(int a, int b) {
	lcd->begin(16, 2);
}

void Interface::update() {
	readButtons();
	if (butPressed[0]) {
		butPressed[0] = 0;
		handle(keyUp);
	} else if (butPressed[1]) {
		butPressed[1] = 0;
		handle(keyDown);
	} else if (butPressed[2]) {
		butPressed[2] = 0;
		handle(keyOk);
	} else if (butPressed[3]) {
		butPressed[3] = 0;
		handle(keyBack);
	}
}

void Interface::tick() {
	menuHandler->tick();
}

