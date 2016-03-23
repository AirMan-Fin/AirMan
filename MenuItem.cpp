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
		isEnd=0;
		if (place == 0) {
			isStart = 1;
		}
	}

	timer = 0;
}
;
void MenuItem::down() {
	if (place < mm.size() - 1) {
		place++;
		isStart=0;
		if(place == mm.size()-1){
			isEnd=1;
		}
	}
	else{

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
	(*current)->enterMenu();
}

void MenuItem::enterMenu(){
	timer=0;
}

void MenuItem::tick() {
	timer++;
	if (timer > 600) {
		timer = 0;
		MenuItem::back();
		(*current)->display();
	}

}

