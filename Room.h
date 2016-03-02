#ifndef ROOM_H
#define ROOM_H

#include "arduino.h"
#include "Temperature.h"

/*
 * !!!!!!!!!!Things to do!!!!!!!!!!!!
 *
 * >min and max for airflow
 *
 * >getAirSupply() where you fine tune you airflow and heating when time passes
 *
 * >selvitä mikä fuckkaa targetTemperaturen ja muut targetit!!
 */
enum roomType {
	classRoom = 0, computerLab = 1, auditorium = 2, office = 3,
};

class Room {
private:
	int area;
	int height;
	int space;

	int PeopleDensity=0;
	int MachineryDensity=0;
	int Windows;


	float sensorTemp=0; //sensor
	float humidity=0;//sensor
	float temperature=21.0; //desired

	float heatloss; //energy balance for room
	int boost = 0;//for how many hours boost


	float targetAirTemperature;
	float targetTime;
	float targetAirflow;

	float airflow; //airflow...
	float differentTemp; //difference between indoor - outdoor

	int wallsize = 0;
	int outerWalls = 0;
	bool recovery = 0;
	int heaterMAX = 30; //30Celcius
	int heaterMIN = 10; //30Celcius
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

	Room(int floor = 40, float height1 = 2.5, float temp = 21.00,
			roomType type = classRoom, int outer = 0, bool reco =0);


	bool update(float Tmp = 21.0, int mon = 1);

	int setCubicValues(int floor = 0, int height1 = 0);// sets cubics
	void setRoomtype(roomType);//sets the rooom type classRoom etc.
	void setTemperatureValues(float);//sets the desired temperature
	void setRecovery(bool);//is there a recovery unit
	void setOuterWalls(int);//sets the amount of outer walls
	void setSensorTemp(float);//sets the temperature sensor value to "temperature"
	void setHumidity(float);//sets humidity sensor value to "humidity"
	void setBoost(int);

	bool  getTargetEnergy();//calculates energy needed to heat room

	float getSpaceValue();//returns space
	float getHeigthValue();//returns height
	roomType getRoomtype();//returns roomType
	float getTemperatureValue();//returns desired temperature
	bool getRecovery();//returns if there is an recovery unit
	int getOuterWalls();// returns the amount of outer walls

};

#endif
