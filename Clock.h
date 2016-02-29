#ifndef CLOCK_H
#define CLOCK_H

#include <cstdio>

class Clock {
public:
	int totalSec; // seconds for year
	int sec;
	int min;
	int hour;
	int day;
	int month;
	int year;

	bool modState;

	Clock() ;

	/*
	 * sets new time
	 */
	void setTime(int sec2 = 0, int min2 = 0, int hour2 = 12, int day2 = 19,
			int month2 = 2, int year2 = 2016) ;

	/*
	 * returns totalSec
	 */
	int getTotalSec() ;

	/*
	 * call this once per second
	 * handles all calculations
	 */
	void tick() ;

};

#endif
