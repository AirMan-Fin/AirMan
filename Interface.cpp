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

Interface::Interface(Room * r, Clock *c, float * temp, int* er, bool * erro, int8_t *modbus, int rs, int en, int d4, int d5, int d6, int d7) {

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
	main = new MainMenu(room,"Main Menu");
	settingsMenu = new SettingsMenu(room,"Settings");
	roomMenu = new RoomMenu(room,"Room Setting");
	dataMenu = new DataMenu(room,"Data", c, temp, er);

	settingsClockMenu = new SettingsClockMenu(room,"Clock", c);
	settingsErrorMenu = new SettingsErrorMenu(room,"Error messages:", erro);
	settingsModbusMenu = new SettingsModbusMenu(room,"Modbus reset:", modbus);

	roomSpaceMenu = new RoomSpaceMenu(room, "Space", 10, 0, 99999);
	roomTypeMenu = new RoomTypeMenu(room,"Type", 0, 0, 3);
	roomTempMenu = new RoomTempMenu(room,"t.Temp", 20, 16, 28, 0.5);
	roomHeatRecoverMenu = new RoomHeatRecoverMenu(room,"Heat Recover", 0, 0, 1);
	roomWindowsMenu = new RoomWindowsMenu(room,"Outer walls", 1, 0, 4);
	roomSetupMenu = new RoomSetupMenu(room,"Setup", roomSpaceMenu, roomTypeMenu, roomTempMenu, roomWindowsMenu, roomHeatRecoverMenu);

	/*
	 * initializing all menus
	 */
	main->init(menuHandler->getpCurrent(), menuHandler->getpVector(), lcd);
	settingsMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
			lcd);
	roomMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(), lcd);
	dataMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(), lcd);

	roomSpaceMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
			lcd);
	roomTypeMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
			lcd);
	roomHeatRecoverMenu->init(menuHandler->getpCurrent(),
			menuHandler->getpVector(), lcd);
	roomWindowsMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
			lcd);
	roomTempMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
			lcd);
	roomSetupMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
				lcd);

	settingsClockMenu->init(menuHandler->getpCurrent(),
			menuHandler->getpVector(), lcd);
	settingsErrorMenu->init(menuHandler->getpCurrent(),
			menuHandler->getpVector(), lcd);
	settingsModbusMenu->init(menuHandler->getpCurrent(),
				menuHandler->getpVector(), lcd);

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
	settingsMenu->addMenuitem(settingsModbusMenu);

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

