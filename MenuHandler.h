#ifndef MENUHANDLER_H
#define MENUHANDLER_H

#include "MenuItem.h"
#include <vector>

enum key {
	keyUp, keyDown, keyOk, keyBack
};

class MenuHandler {
private:
	MenuItem * current;
	std::vector<MenuItem> menus;
	std::vector<MenuItem *> prevs;
public:
	MenuHandler();

	/*
	 * adds first menu
	 */
	void addMenu(MenuItem &mm);

	/*
	 * handles keypressed to right function
	 */
	void handle(key keyy);

	/*
	 * calls current menus disply
	 */
	void display();

	/*
	 * timer for menus
	 */
	void tick();

	/*
	 * getterit
	 */
	MenuItem ** getpCurrent();
	std::vector<MenuItem*> * getpVector();

};

#endif

