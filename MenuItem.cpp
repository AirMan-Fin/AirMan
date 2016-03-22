#include <vector>
#include "MenuItem.h"
#include "LiquidCrystal.h"
#include <string>
#include <cstdio>

void MenuItem::init(MenuItem** mm, std::vector<MenuItem*> * mv,
		LiquidCrystal *l) {
	current = mm;
	prevs2 = mv;
	lcd = l;
	timer = 0;
}
MenuItem::MenuItem() {
	place = 0;
	isEnd=0;
	isStart=1;
}

void MenuItem::addMenuitem(MenuItem *mmm) {
	mm.push_back(mmm);
}

void MenuItem::up() {
	if (place > 0) {
		place--;
		if (place > 0) {
			isEnd = 0;
		} else {
			isEnd = 1;
		}
	}

	timer = 0;
}
;
void MenuItem::down() {
	if (place < mm.size() - 1) {
		place++;
		if (place < mm.size() - 1) {
			isStart = 0;
		} else {
			isStart = 1;
		}
	}
	timer = 0;
}
;
void MenuItem::ok() {

	setCurrent(mm[place]);
	timer = 0;

}
;
void MenuItem::back() {
	if (prevs2->size() > 0) {
		(*current) = (*prevs2)[prevs2->size() - 1];
		prevs2->pop_back();
	}
}
void MenuItem::display() {

	lcd->clear();
	lcd->print(name);
}
void MenuItem::func() {
	lcd->clear();
	lcd->print(name);
	lcd->print(":");
	lcd->setCursor(0, 1);
	lcd->print("-");
}

void MenuItem::setCurrent(MenuItem *c) {
	prevs2->push_back(*current);
	(*current) = c;
}

void MenuItem::tick() {
	timer++;
	if (timer > 600) {
		timer = 0;
		MenuItem::back();
		(*current)->display();
	}

}

