#include "Interface.h"

class Interface {
private:
	bool butPressed[4];
	Room room;

	void readButtons() {
		for (int a = 0; a < 4; a++) {
			if(button[a].read()){
				butPressed[a] = 1;
			}
		}
	}
	void handle(key){

	}

public:
	Interface(Room * r, int a = 8, int b = 9, int c = 10, int d = 11,
			int e = 12, int f = 13) {
		lcd = new LiquidCrystal(a, b, c, d, e, f);
		room = r;
	}
	~Interface() {
		delete room;
		delete lcd;
	}
	void initButtons(int b1, int b2, int b3, int b4) {
		int temp[4] = { b1, b2, b3, b4 };
		for (int a = 0; a < 4; a++) {
			button[a] = new Button(temp[a]);
		}
	}
	void lcdBegin(int a = 16, int b = 2) {
		lcd->begin(16, 2);
	}

	void update() {
		readButtons();
		if(butPressed[0]){
			butPressed [0]=0;
			handle(up);
		}
		else if(butPressed[1]){
			butPressed [1]=0;
			handle(down);
		}
		else if(butPressed[2]){
			butPressed [2]=0;
			handle(enter);
		}
		else if(butPressed[3]){
			butPressed [3]=0;
			handle(back);
		}
	}

};
