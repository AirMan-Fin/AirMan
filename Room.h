#ifndef ROOM_H
#define ROOM_H

#include "arduino.h"
#include "Temperature.h"
#include "Eeprom.h"
#define testMode
#define areaMem 10// 4bytes
#define heightMem 14// 2bytes
#define outerwallMem 16// byte
#define recoveryMem 17 //byte
#define roomTypeMem 18 //byte



enum roomType {
	classRoom = 0, computerLab = 1, auditorium = 2, office = 3,
};

class Room {
private:
	Eeprom *eeprom;
	float area=40;
	float height=3;
	float space;
	float MAXairflow=0.2,MINairflow=0.01;
	float userHeaterMIN=17;

	float PeopleDensity=0;
	float MachineryDensity=0;
	float Windows;
	float targetTime=1800;//standard heating time 1800 increased if airflow exeeds maximum value


	float sensorTemp=0; //sensor
	float humidity=0;//sensor
	float temperature=21.0; //desired

	float heatTotal=0; //energy balance for room
	float boost = 0;//for how many hours boost
	bool powersave=0;



	float blowingTemperature=0;
	float targetAirflow=0;
	float tMeas=0;

	float airflow; //airflow...
	float differentTemp; //difference between indoor - outdoor

	float wallsize = 0;
	float outerWalls = 0;
	bool recovery = 0;
	float heaterMAX = 30; //30Celcius
	bool err = true;
	float MAXradiatorTemp=100;
	/*
	 * http://www.engineeringtoolbox.com/psychrometric-chart-mollier-d_27.html
	 * temperature*humidity
	 */

	int month = 1;

	float monthTempHelsinki[12] { -2, -3, 3, 10, 15, 19, 22, 21, 16, 9, 4, 1 };
	roomType room;

	float getTempDiff(float, int);
	void getAirSupplyTemp();
	void calculateAirflow();
	void getHeatLoss();
	bool  getTargetEnergy();//calculates energy needed to heat room
	void trimmer();
public:

	Room(Eeprom *e, int floor = 40, float height1 = 2.5, float temp = 21.00,
			roomType type = classRoom, int outer = 0, bool reco =0);


	bool update(float Tmp = 21.0, int mon = 1, float humidity1=0);

	int setCubicValues(float floor = 0, float height1 = 0);// sets cubics
	void setRoomtype(roomType);//sets the rooom type classRoom etc.
	void setTemperatureValues(float);//sets the desired temperature
	void setRecovery(bool);//is there a recovery unit
	void setOuterWalls(float);//sets the amount of outer walls
	void setSensorTemp(float);//sets the temperature sensor value to "temperature"
	void setHumidity(float);//sets humidity sensor value to "humidity"
	void setBoost(float);
	void setMAXairflow(float);
	void setMINairflow(float);

	void setPowerSave(bool b);



	float getSpaceValue();//returns space
	float getHeigthValue();//returns height
	roomType getRoomtype();//returns roomType
	float getTemperatureValue();//returns desired temperature
	bool getRecovery();//returns if there is an recovery unit
	int getOuterWalls();// returns the amount of outer walls
	float getAirflow();
	float getBlowingTemp();

};

#endif
