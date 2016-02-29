#ifndef FAN_H
#define FAN_H


class Fan {
private:
	bool autoMode;
	int speed;
public:
	Fan();

	void update(int);
	void setSpeed();

};

#endif
