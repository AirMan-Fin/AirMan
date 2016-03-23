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

#define photoOk

#include <cstdio>
#include <cr_section_macros.h>

// TODO: insert other include files here
#include "arduino.h"
#include "Fan.h"
#include "Heater.h"
#include "Temperature.h"
#include "Photoresistor.h"
#include "Pressure.h"
#include "Humidity.h"
#include "InterFace.h"
#include "Clock.h"
#include "Buzzer.h"
#include "Eeprom.h"
#include "I2C.h"

// TODO: insert other definitions and declarations here
#define TICKRATE 1000
#define but1pin 3
#define but2pin 5
#define but3pin 6
#define but4pin 7

#define heartbeat

//unsigned long long milliss;
bool flTick = 0;
bool clockTick = 0;
bool msTick = 0;
bool flInterfaceTick = 0;
bool fanTick=0;
Millis *mm;

extern "C" {
void SysTick_Handler() {
	mm->tick();
	if (mm->millis() % 2000 == 0) { // every 10 sec
		if (flTick) {
			printf("mis fl\n");
		}
		flTick = 1;
	}
	if (mm->millis() % 4000 == 0) { // every 4 sec
		if (fanTick) {
			printf("mis fan\n");
		}
		fanTick = 1;
	}
	if (mm->millis() % 1000 == 0) { // every sec
		if (clockTick) {
			printf("mis cl\n");
		}
		clockTick = 1;
	}
	if (mm->millis() % 100 == 0) { // every 100 millisec
		if (msTick) {
			printf("mims\n");
		}
		msTick = 1;
	}
	if (mm->millis() % 10 == 0) { // every 10 millisec
		if (flInterfaceTick) {
			//printf("flI\n");
		}
		flInterfaceTick = 1;
	}
}
}

int main(void) {
	printf("go!!\n");

#if defined (__USE_LPCOPEN)
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	Board_Init();
	Board_LED_Set(0, true);
#endif
#endif
	mm = new Millis();
	mm->tick();
	Buzzer buzzer(4);

	enRit();
	// TODO: insert code here
	I2C i2c(0, 100000);
	Eeprom eeprom(1);
	Heater heater;
#ifdef photoOk
	Photoresistor photo(0);
#endif

	Room room(&eeprom, 40, 3);
	Clock clock;
	bool errors[] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // modbus |  pressure | too high fan input | humi-temp |

	int8_t modbusConnection = 3;
	Pressure pres(&i2c);
	Humidity humi(&i2c);

	float tempData = 0;
	float humidityData = 0;
	float pressureData = 0;

	Interface interface(&room, &clock, &tempData, &humidityData, errors,
			&modbusConnection, 8, 9, 10, 11, 12, 13);
	interface.lcdBegin();
	interface.initButtons(but1pin, but2pin, but3pin, but4pin);

	SysTick_Config(Chip_Clock_GetSysTickClockRate() / TICKRATE);
	Fan fan(mm);
	printf("Begin...\n");

	while (1) {

		if (flInterfaceTick) { // once every 10 ms
			flInterfaceTick = 0;
			interface.update();
			//buzzer.tick();
		}
		if (msTick) {   //once every 100ms
			msTick = 0;
			interface.tick();

		}
		if (clockTick) { // once every second
			clockTick = 0;
			clock.tick();

#ifdef photoOk
			photo.update();
			if(photo.getLightState()) {
				room.setPowerSave(1);
			}
			else {
				room.setPowerSave(0);
			}
#endif

		}
		if (fanTick) {
			fanTick = 0;
#ifdef heartbeat
			if (pres.getValue(&pressureData)) { // read pressure sensor
				//printf("%d\n",pressureData);
				errors[1] = 0;
			} else {
				errors[1] = 1;
			}
			fan.update(pressureData);
#endif
		}
		if (flTick) { // once every 10 second
			flTick = 0;
			//buzzer.beeb(50);
			if (humi.getValue(&humidityData, &tempData)) {
				errors[3] = 0;
				//printf("%3.2f\n", humidityData);
			} else {
				errors[3] = 1;
			}

			//printf("%3.2f \n",tempData);

			//printf("%3.2f, %3.2f \n", tempData, humidityData);
			room.update(tempData, clock.month, humidityData);
			//heater.update(room.getBlowingTemp());

			if (modbusConnection > 0) {
				errors[0] = 0;
				modbusConnection--;
				int ss = mm->mmm;
				//int o = fan.setAirFlow(room.getAirflow() * 400, pressureData);
				int o = fan.setAirFlow(0.2, pressureData);
				if (o == 0 || o == 1) {
					modbusConnection = 3;
				} else if (o == 1 || o == 11) { //communication problems
					errors[0] = 1;
				} else if (o == 10) {  //too high input value
					errors[2] = 1;
				} else {
					errors[2] = 0;
				}
				ss = mm->mmm - ss;
				//printf("time: %d ", ss);
				//printf("result: %d\n", o);
			} else {

				errors[0] = 1;
			}
		}

	}

	return 0;
}
