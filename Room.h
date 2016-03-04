#ifndef ROOM_H
#define ROOM_H

#include "arduino.h"
#include "Temperature.h"
#include "Eeprom.h"

#define areaMem  10// save address for area- 4 bytes
#define heightMem 14 // save address for height- 2 byte
#define heaterMAXMem 16 // save address for heatermax- 1 byte
#define recoveryMem 17 // save address for recovery- 1 byte
#define outerwallMem 18 // save address for outerwalls- 1 byte
#define roomTypeMem 19 // save address for room type- 1 byte

#define testMode
/*
 * !!!!!!!!!!Things to do!!!!!!!!!!!!
 *
 * >manageAirSupply() where you fine tune you airflow and heating when time passes
 *
 */
enum roomType {
	classRoom = 0, computerLab = 1, auditorium = 2, office = 3,
};

class Room {
private:
	Eeprom *eeprom;

	float area=40;
	float height=3;
	float space;
	float MAXairflow=10,MINairflow=0;
	float userHeaterMIN=15;

	float PeopleDensity=0;
	float MachineryDensity=0;
	float Windows;
	float time;//standard heating time 1800 increased if airflow exeeds maximum value


	float sensorTemp=0; //sensor
	float humidity=0;//sensor
	float temperature=21.0; //desired

	float heatTotal=0; //energy balance for room
	float boost = 0;//for how many hours boost



	float targetAirTemperature=0;
	float targetTime;
	float targetAirflow=0;

	float airflow; //airflow...
	float differentTemp; //difference between indoor - outdoor

	float wallsize = 0;
	float outerWalls = 0;
	bool recovery = 0;
	float heaterMAX = 30; //30Celcius
	float heaterMIN = 10; //30Celcius
	bool err = true;

	/*
	 * http://www.engineeringtoolbox.com/psychrometric-chart-mollier-d_27.html
	 * temperature*humidity
	 */

	int month = 1;

	float monthTempHelsinki[12] { -2, -3, 3, 10, 15, 19, 22, 21, 16, 9, 4, 1 };
	roomType room;

	float getTempDiff(float, int);
	void getAirSupplyTemp();
	void getAirflow();
	void getHeatLoss();
public:

	Room(Eeprom *ee,int floor = 40, float height1 = 2.5, float temp = 21.00,
			roomType type = classRoom, int outer = 0, bool reco =0);


	bool update(float Tmp = 21.0, int mon = 1);

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

	void powerSave();
	bool  getTargetEnergy();//calculates energy needed to heat room

	void trimmer();
	float getSpaceValue();//returns space
	float getHeigthValue();//returns height
	roomType getRoomtype();//returns roomType
	float getTemperatureValue();//returns desired temperature
	bool getRecovery();//returns if there is an recovery unit
	int getOuterWalls();// returns the amount of outer walls

};

#endif
