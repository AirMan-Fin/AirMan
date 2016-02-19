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

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "arduino.h"
#include "InterFace.h"
#include "Clock.h"

// TODO: insert other definitions and declarations here
#define TICKRATE 1000
#define but1pin 0
#define but2pin 1
#define but3pin 2
#define but4pin 3

unsigned long long millis;
bool flTick = 0;
bool flInterfaceTick = 0;

extern "C" {
void SysTick_Handler() {
	millis++;
	if (millis % 1000 == 0) {
		//flTick = 1;
	}
	if (millis % 10 == 0) {
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
	Heater heater;

	//Room room(15000, 2700);
	Clock clock;

	Interface interface(&clock,8,9,10,11,12,13);
	interface.lcdBegin();
	interface.initButtons(but1pin, but2pin, but3pin, but4pin);

	while (1) {

		if (flInterfaceTick) {
			flInterfaceTick = 0;
			interface.update();
		}

		if (flTick) {
			flTick = 0;
			clock.tick();
			interface.tick();
			//room.update();
			heater.update();
			//fan.update(room.getAirflow());
		}
	}

	return 0;
}
