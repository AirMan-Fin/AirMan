#ifndef ROOM_H
#define ROOM_H

#include "arduino.h"

class Room{
private:
	int area;
	int height;
	int space;

	int people;
	int airVariable;
	int indoorTemp;//sensor
	float temperature;//desired
	float heatloss;
	float airflow;
	int temperate;
	int humidity;
	int differentTemp;
	int wallsize=0;
	float radiatorHeat=0;
	int outerWalls=0;
	bool recovery=0;
	int heaterMAX=30;//30Celcius
	bool err=true;
	/*
	 * http://www.engineeringtoolbox.com/psychrometric-chart-mollier-d_27.html
	 * temperature*humidity
	 */

	float monthTempHelsinki[12]{-2, -3, 3, 10, 15, 19, 22, 21, 16, 9, 4 , 1};
	enum roomType{
		classRoom=0,
		computerLab=1,
		auditorium=2,
		office=3,
	};
	roomType room;

	void calculate();// not used
public:
	Room(int floor = 40, float height1=2.5, float temp = 21.00, roomType type=classRoom, int outer=0);
	bool update(float, int);

	float getAirflow(float people=0,float windows=0, int machinery=0);
	float getVentHeatLoss(bool recovery=0);

	int setCubicValues(int floor=0, int height1=0);
	int setPersonValues(int);
	int setFurnitureValues(int);
	float setTemperatureValues(float,float,float);
	float outerWallHeatLoss(int outw=0);
	float getTempDiff(float, int);
	float getAirSupplyTemp(float);
	void setRoomtype(roomType);
	float setTemperatureValues(float);
	void setindoorTemp(float);
	void setRecovery(bool);
	void setOuterWalls(int);
};

#endif
