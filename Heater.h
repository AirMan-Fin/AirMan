#ifndef HEATER_H
#define HEATER_H



class Heater{
private:
	int temp;
public:
	Heater();

	/*
	 * check if working as should
	 * param: new target temperature
	 */
	bool update(int t=0);

	/*
	 * set target temperature for air in pipe near heater
	 */
	void setTemp(int);
};

#endif
