#ifndef EEPROM_H
#define EEPROM_H

#include "board.h"

class Eeprom{
private:
	uint32_t buffer32 [8];
	uint8_t *ptr;

public:

	/*
	 * initializes eeprom memory
	 */
	Eeprom(bool a=0);

	/*
	 * writes to eeprom
	 * params: address to start from, data to write, how many addresses to write
	 * returns true if ok
	 */
	bool write(int address, uint8_t* buff, int amount);

	/*
	 * read from eeprom
	 * params: address to start from, where to save data, how many addresses to read
	 */
	bool read(int add, uint8_t* buff, int amount);


};

#endif
