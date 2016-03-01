#ifndef FAN_H
#define FAN_H


class Fan {
private:
	bool autoMode;
	int speed;
public:
	Fan(int);

	void update(int);
	bool setFrequency(ModbusMaster& node, uint16_t);

};

#endif
