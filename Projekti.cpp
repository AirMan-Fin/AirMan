/*
 ===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cstdio>
#include <cr_section_macros.h>

// TODO: insert other include files here
#include "arduino.h"
#include "Fan.h"
#include "Heater.h"
#include "Temperature.h"
#include "Pressure.h"
#include "Humidity.h"
#include "InterFace.h"
#include "Clock.h"
#include "Eeprom.h"

// TODO: insert other definitions and declarations here
#define TICKRATE 1000
#define but1pin 0
#define but2pin 1
#define but3pin 2
#define but4pin 3

unsigned long long milliss;
bool flTick = 0;
bool clockTick = 0;
bool msTick = 0;
bool flInterfaceTick = 0;

extern "C" {
void SysTick_Handler() {
	milliss++;
	if (milliss % 1000 == 0) { // every 10 sec
		flTick = 1;
	}
	if (milliss % 1000 == 0) { // every sec
		clockTick = 1;
	}
	if (milliss % 1000 == 0) { // every 100 millisec
		msTick = 1;
	}
	if (milliss % 10 == 0) { // every 10 millisec
		flInterfaceTick = 1;
	}
}
}

int main(void) {

#if defined (__USE_LPCOPEN)
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	Board_Init();
	Board_LED_Set(0, true);
#endif
#endif

	SysTick_Config(Chip_Clock_GetSysTickClockRate() / TICKRATE);
	enRit();
	// TODO: insert code here
	//Fan fan;

	Eeprom eeprom(1);

	Heater heater;

	Room room(&eeprom, 40, 3);
	Clock clock;
	int error = 0;
	bool errors[] = { 0, 0, 0, 0, 0, 0 };

	Temperature temp(0);
	//Pressure pres(0);
	Humidity humi(0);

	float tempData;
	float humidityData;
	float pressureData;

	Interface interface(&room, &clock, &tempData, &error, errors, 8, 9,
			10, 11, 12, 13);
	interface.lcdBegin();
	interface.initButtons(but1pin, but2pin, but3pin, but4pin);

	while (1) {

		if (flInterfaceTick) { // once every 10 ms
			flInterfaceTick = 0;
			interface.update();
		}
		if (msTick) {
			msTick = 0;
			interface.tick();

		}
		if (clockTick) { // once every second
			clockTick = 0;
			clock.tick();
		}
		if (flTick) { // once per 10 second
			flTick = 0;
			if (!temp.getValue(&tempData)) {
				error = 1;
				errors[0] = 0;
			} else {
				error = 0;
				errors[0] = 0;
			}
			if (!humi.getValue(&humidityData, &tempData)) {
				error = 2;
				errors[1] = 1;
			} else {
				error = 0;
				errors[1] = 0;
			}
			if (0) { // read pressure sensor
				error = 3;
				errors[2] = 1;
			} else {
				error = 0;
				errors[2] = 0;
			}
			//printf("%d\n",data);
			//room.update(tempData, clock->month, humidityData);
			//heater.update(room.getHeatflow());
			//fan.update(room.getAirflow());
		}
	}

	return 0;
}
