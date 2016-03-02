

#ifndef LCD_PORT_H_
#define LCD_PORT_H_

#include <string>
#include <sstream>

/* Definitions needed by Arduino LiquidCrystal library
 *
 * This file must be included after chip.h and/or board.h if either of them is also included
 * */

//#define OUTPUT 1
//#define INPUT 0
#define HIGH 1
#define LOW 0

//template<typename T> std::string toStr(T);

void enRit();
void digitalWrite(int pin, int val);
bool digitalRead(int pin);
void pinMode(int pin, int mode);
void delayMicroseconds(int us);
void delay(int ms);


class Button{
private:
	int pin;
	int prevState;
public:
	Button(int);
	bool read();
};

static volatile bool adcdone = false;

class AnalogPort{
private:
	int pin;
public:
	AnalogPort(int);
	int read();
};

template<typename T> std::string toStr(T t) {
	std::stringstream ss;
	ss << t;
	return ss.str();
}

#endif /* LCD_PORT_H_ */
