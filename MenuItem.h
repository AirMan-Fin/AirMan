#ifndef MENUITEM_H_
#define MENUITEM_H_

#include <vector>
#include <string>
#include "Room.h"
#include "LiquidCrystal.h"

class MenuItem {
public:
	std::string name;

	MenuItem **current;
	std::vector<MenuItem*> *prevs2;

	std::vector<MenuItem *> mm;

	unsigned int place;
	int timer;

	LiquidCrystal *lcd;
	Room *room;

	void init(MenuItem **mm, std::vector<MenuItem*> *, LiquidCrystal *l);
	MenuItem();

	/*
	 * adds new submenu for this menu
	 */
	void addMenuitem(MenuItem *);

	/*
	 * returns current menuitem
	 */
	MenuItem * getCurrent();

	/*
	 * sets current menu
	 */
	void setCurrent(MenuItem *c);

	/*
	 * virtual default methods for buttons
	 */
	virtual void up();
	virtual void down();
	virtual void ok();
	virtual void back();

	/*
	 * default method for displaying info
	 */
	virtual void display();
	/*
	 * default help method for displaying first row of lcd
	 *
	 * current name
	 * -...
	 */
	void func();

	/*
	 * default tick method, after 30 seconds, returns back
	 */
	virtual void tick();

};

#endif
