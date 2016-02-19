#include "MenuHandler.h"
#include <vector>

MenuHandler::MenuHandler() {
}

void MenuHandler::addMenu(MenuItem &mm) {
	if (menus.size() == 0) {
		current = &mm;
	}
	menus.push_back(mm);
}

void MenuHandler::handle(key keyy) {
	switch (keyy) {
	case keyUp:
		current->up();
		break;

	case keyDown:
		current->down();
		break;

	case keyOk:
		current->ok();
		break;

	case keyBack:
		current->back();
		break;
	}
}
void MenuHandler::display() {
	current->display();
}

void MenuHandler::tick(){
	current->tick();
}

MenuItem ** MenuHandler::getpCurrent() {
	return &current;
}

std::vector<MenuItem*> * MenuHandler::getpVector() {
	return &prevs;
}


