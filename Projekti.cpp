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

//  insert other include files here
#include "arduino.h"
#include "Fan.h"
#include "Photoresistor.h"
#include "Pressure.h"
#include "Humidity.h"
#include "InterFace.h"
#include "Clock.h"
#include "Eeprom.h"
#include "I2C.h"
//#include "Buzzer.h"

// insert other definitions and declarations here
#define TICKRATE 1000
#define but1pin 3
#define but2pin 5
#define but3pin 6
#define but4pin 7

#define heartbeat

bool flTick = 0;
bool clockTick = 0;
bool msTick = 0;
bool flInterfaceTick = 0;
bool fanTick = 0;
Millis *mm;

extern "C" {
void SysTick_Handler() {
	mm->tick();
	if (mm->millis() % 10000 == 0) { // every 10 sec
		flTick = 1;
	}
	if (mm->millis() % 4000 == 0) { // every 4 sec
		fanTick = 1;
	}
	if (mm->millis() % 1000 == 0) { // every sec
		clockTick = 1;
	}
	if (mm->millis() % 100 == 0) { // every 100 millisec
		msTick = 1;
	}
	if (mm->millis() % 10 == 0) { // every 10 millisec
		flInterfaceTick = 1;
	}
}
}

int main(void) {

#if defined (__USE_LPCOPEN)
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	Board_Init();
#endif
#endif

	mm = new Millis();//millisecond holder. now we can send it to modbus class
	enRit();//enable RIT timer
	I2C i2c(0, 100000); // start I2C object
	Eeprom eeprom(1); // start eeprom object

	Photoresistor photo(0);// light monitoring object

	Room room(&eeprom, 40, 3); // here is most of our calculations
	Clock clock; //  time keeper object

	//list of errors
	bool errors[] = { 0, 0, 0, 0, 0, 0, 0, 0}; // modbus |  pressure | too high fan input | humi-temp |

	int8_t modbusConnection = 3;
	Pressure pres(&i2c); //reads pressure sensor
	Humidity humi(&i2c); //reads temperature and humidity

	//Buzzer buzzer(4); // annoying beeber

	float tempData = 0;
	float humidityData = 0;
	float pressureData = 0;
	bool nDisabled = 0;

	SysTick_Config(Chip_Clock_GetSysTickClockRate() / TICKRATE);

	Fan fan(mm, &eeprom); //modbus communicating object
	Interface interface(&room, &clock, &tempData, &humidityData, &fan, errors, // object that handles input-output with user
			&nDisabled, &modbusConnection, 8, 9, 10, 11, 12, 13);
	interface.lcdBegin();  // start lcd
	interface.initButtons(but1pin, but2pin, but3pin, but4pin); // initializes buttons

	while (1) {

		if (flInterfaceTick) { // once every 10 ms
			flInterfaceTick = 0;
			interface.update(); // check button states
		}
		if (msTick) {   //once every 100ms
			msTick = 0;
			interface.tick(); // timer things in interface

		}
		if (clockTick) { // once every second
			clockTick = 0;
			clock.tick(); // for time keeping

#ifdef photoOk
			photo.update();  // read light value in room
			if (!nDisabled) {
				if (clock.hour < 6 || clock.hour > 22) { // if not disabled power save, and its night and lights are out, enter powersave mode
					if (photo.getLightState()) {
						room.setPowerSave(1);
					} else {
						room.setPowerSave(0);
					}
				}
			}
#endif
		}
		if (fanTick) { //once every 4 second
			fanTick = 0;
#ifdef heartbeat
			errors[1] = (pres.getValue(&pressureData)) ? 0 : 1; // read pressure sensor
			//printf("%3.2f \n",pressureData);
			errors[0] = fan.update(pressureData) ? 0 :1; // send value to frequency controller, also the heartbeat
#endif
		}
		if (flTick) { // once every 10 second
			flTick = 0;
			errors[3] = (humi.getValue(&humidityData, &tempData)) ? 0 : 1; // reading values from humidity-temperature sensor

			room.update(tempData, clock.month, humidityData); // sending readings and calculating target airflow and target temperature

			//here we should send target temperature to heater, if we had one

			if (modbusConnection > 0) { /// data to fan, if we have connection
				errors[0] = 0;
				modbusConnection--;
				int o = fan.setAirFlow(room.getAirflow(), pressureData);
				if (o == 0) {
					modbusConnection = 3;
				} else if (o == 1 || o == 11) { //communication problems
					errors[0] = 1;
				} if (o == 10) {  //too high input value
					errors[2] = 1;
				} else {
					errors[2] = 0;
				}
			} else {
				errors[0] = 1;
			}
		}
	}

	return 0;
}
