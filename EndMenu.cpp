#include "EndMenu.h"



EndMenu::EndMenu() {

	}

	void EndMenu::ok() {
		if (modState) {
			value=temp;
		} else {

		}
		modState = !modState;
	}
	void EndMenu::up() {
		if (modState) {
			if (temp < max)
			temp+=intervall;
		}
	}
	void EndMenu::down() {
		if (modState) {
			if (temp > min)
			temp-=intervall;
		}
	}
	void EndMenu::back() {
		modState = 0;
		temp=value;
		MenuItem::back();
	}
	void EndMenu::display() {

		func();
		lcd->print(text);
		lcd->print(":");
		if (modState)
			lcd->print("{");
		else
			lcd->print(" ");
		lcd->print(toStr(temp).c_str());
		if (modState)
			lcd->print("}");

	}

	int EndMenu::getInt(){
		return (int) value;
	}

	float EndMenu::getFloat(){
		return value;
	}


