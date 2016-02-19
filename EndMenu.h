#ifndef ENDMENU_H
#define ENDMENU_H

#include <string>
#include "MenuItem.h"
#include "arduino.h"

class EndMenu: public MenuItem {
public:
	bool modState = 0;

	std::string text;
	float max;
	float min;
	float value;
	float temp;
	float intervall;



	/*
	 * params: name, value, min value, max value, chance interval
	 * set in inheriting class
	 */
	EndMenu();

	/*
	 * different key actions
	 * default functions
	 */
	virtual void ok() ;
	virtual void up() ;
	virtual void down() ;
	virtual void back();

	/*
	 * default display func
	 */
	virtual void display() ;

	/*
	 * return value
	 */
	int getInt();
	float getFloat();

};



#endif
