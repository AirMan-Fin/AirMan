#include "Interface.h"

void Interface::readButtons() {
	for (int a = 0; a < 4; a++) {
		if (button[a]->read()) {
			butPressed[a] = 1;
		}
		if (button[a]->isPressed()) {
			butPressed[a] = 1;
		}
	}
}
void Interface::handle(key k) {
	menuHandler->handle(k);
	menuHandler->display();
}

Interface::Interface(Room * r, Clock *c, float * temp, float *humi, Fan * f, bool * er, bool *dis,
		int8_t *modbus, int rs, int en, int d4, int d5, int d6, int d7) {

	room = r;
	fan=f;
	fan->reset();
	fan->getPipeArea();
	butPressed[0] = 0;
	butPressed[1] = 0;
	butPressed[2] = 0;
	butPressed[3] = 1;
	lcd = new LiquidCrystal(rs, en, d4, d5, d6, d7);
	//room = r;

	menuHandler = new MenuHandler();

	/*
	 * creating all required menus
	 */
	main = new MainMenu(room, "Main Menu");
	settingsMenu = new SettingsMenu(room, "Settings");
	roomMenu = new RoomMenu(room, "Room setup");
	dataMenu = new DataMenu(room, "Data", c, temp, humi, er);

	settingsClockMenu = new SettingsClockMenu(room, "Clock", c);
	settingsErrorMenu = new SettingsErrorMenu(room, "Errors", er);
	settingsDisableMenu = new SettingsDisableMenu(room, "Dis. pow. save", dis);
	settingsModbusMenu = new SettingsModbusMenu(room, "Modbus reset", modbus);
	settingsResetMenu = new SettingsResetMenu(room, fan, "Factory reset" );

	roomSpaceMenu = new RoomSpaceMenu(room, "Space", 10, 0, 99999);
	roomTypeMenu = new RoomTypeMenu(room, "Type", 0, 0, 3);
	roomTempMenu = new RoomTempMenu(room, "Temperature", 20, 17, 24, 0.5);
	roomDefaultMenu = new RoomDefaultMenu(room, "Def. airflow", 0, 0, 10, 1);
	roomPipeAreaMenu = new RoomPipeAreaMenu(room,"Vent area", fan);
	roomHeatRecoverMenu = new RoomHeatRecoverMenu(room, "Heat Recover", 0, 0,
			1);
	roomWindowsMenu = new RoomWindowsMenu(room, "Outer walls", 1, 0, 4);
	roomSetupMenu = new RoomSetupMenu(room, "Setup all", roomSpaceMenu,
			roomTypeMenu, roomTempMenu, roomWindowsMenu, roomHeatRecoverMenu);

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
	roomDefaultMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
				lcd);
	roomPipeAreaMenu->init(menuHandler->getpCurrent(), menuHandler->getpVector(),
					lcd);

	settingsClockMenu->init(menuHandler->getpCurrent(),
			menuHandler->getpVector(), lcd);
	settingsErrorMenu->init(menuHandler->getpCurrent(),
			menuHandler->getpVector(), lcd);
	settingsModbusMenu->init(menuHandler->getpCurrent(),
			menuHandler->getpVector(), lcd);
	settingsDisableMenu->init(menuHandler->getpCurrent(),
				menuHandler->getpVector(), lcd);
	settingsResetMenu->init(menuHandler->getpCurrent(),
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
	settingsMenu->addMenuitem(settingsDisableMenu);
	settingsMenu->addMenuitem(settingsErrorMenu);
	settingsMenu->addMenuitem(settingsModbusMenu);
	settingsMenu->addMenuitem(settingsResetMenu);

	roomMenu->addMenuitem(roomSetupMenu);
	roomMenu->addMenuitem(roomTempMenu);
	roomMenu->addMenuitem(roomSpaceMenu);
	roomMenu->addMenuitem(roomTypeMenu);
	roomMenu->addMenuitem(roomHeatRecoverMenu);
	roomMenu->addMenuitem(roomWindowsMenu);
	roomMenu->addMenuitem(roomDefaultMenu);
	roomMenu->addMenuitem(roomPipeAreaMenu);

	/*
	 * first display
	 */


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
	menuHandler->display();
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
	for (int a = 0; a < 4; a++) {
		button[a]->tick();
	}
}

/*
 * params: 1 is up, 2 down
 */
void drawArrow(LiquidCrystal &lcd, int object, int column, int row) {
	const uint8_t upArrow[8][8] = { { 0x04, 0x0E, 0x1F, 0x04, 0x04, 0x00, 0x00,
			0x00 }, { 0x04, 0x0E, 0x1F, 0x04, 0x04, 0x00, 0x00, 0x00 }, { 0x04,
			0x0E, 0x1F, 0x04, 0x04, 0x00, 0x00, 0x00 }, { 0x04, 0x0E, 0x1F,
			0x04, 0x04, 0x00, 0x00, 0x00 } };
	const uint8_t downArrow[8][8] = {
			{ 0x0, 0x0, 0x0, 0x4, 0x4, 0x1f, 0xe, 0x4 }, { 0x0, 0x0, 0x0, 0x4,
					0x4, 0x1f, 0xe, 0x4 }, { 0x0, 0x0, 0x0, 0x4, 0x4, 0x1f, 0xe,
					0x4 }, { 0x0, 0x0, 0x0, 0x4, 0x4, 0x1f, 0xe, 0x4 }

	};
	uint8_t length = 1;
	uint8_t size;
	int value = 1;

	if (length < 5)
		length = 5;
	switch (object) {
	case 1:	//up arrow
		for (int i = 0; i < 8; i++) {
			lcd.createChar(i, (uint8_t *) upArrow[i]);
		}
		break;
	case 2:	//up arrow
		for (int i = 0; i < 8; i++) {
			lcd.createChar(i, (uint8_t *) downArrow[i]);
		}
		break;

	}
	size = length / 5 + (length % 5 > 0 ? 1 : 0);

	if (value < 0)
		length = 0;
	if (value > length)
		value = length;
	else {
		uint8_t full = value / 5;
		uint8_t rest = value % 5;
		uint8_t count = size;
		lcd.setCursor(column, row); // move cursor back to visible area
		while (count) {
			if (full > 0) {
				lcd.write(4);
				full--;
			} else if (rest > 0) {
				lcd.write(rest);
				rest = 0;
			} else {
				lcd.write(32);
			}
			count--;
		}
	}
	lcd.setCursor(0, 0);
}
