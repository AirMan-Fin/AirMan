#include "Clock.h"

Clock::Clock() {
	modState=0;
}

void Clock::setTime(int sec2, int min2, int hour2, int day2, int month2,
		int year2) {
	sec = sec2;
	min = min2;
	hour = hour2;
	day = day2;
	month = month2;
	year = year2;
}
int Clock::getTotalSec() {
	return totalSec;
}

void Clock::tick() {
	if (!modState) {
		sec++;
		totalSec++;

		if (sec > 59) {
			sec = 0;
			min++;

			if (min > 59) {
				min = 0;
				hour++;

				if (hour > 23) {
					hour = 0;
					day++;

					if (day > 28 && month == 2) {
						month++;
						day = 0;
					} else if (day > 30 && month < 8 && month % 2 == 0) {
						month++;
						day = 0;
					} else if (day > 31 && month < 8 && month % 2 == 1) {
						month++;
						day = 0;
					} else if (day > 30 && month > 7 && month % 2 == 1) {
						month++;
						day = 0;
					} else if (day > 31 && month > 7 && month % 2 == 0) {
						month++;
						day = 0;
					}
					if (month > 12) {
						month = 0;
						year++;
						totalSec = 0;
					}
				}
			}
		}
	}
}
