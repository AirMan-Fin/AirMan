#include "Eeprom.h"

Eeprom::Eeprom(bool a) {
	/* Enable EEPROM clock and reset EEPROM controller */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_EEPROM);
	Chip_SYSCTL_PeriphReset(RESET_EEPROM);
	ptr = (uint8_t *) buffer32;
}

bool Eeprom::write(int address, uint8_t *data, int amount) {

	/* Data to be written to EEPROM */
	int ret_code = Chip_EEPROM_Write(address, data, amount);

	/* Error checking */
	if (ret_code == IAP_CMD_SUCCESS) {
		return 1;
	} else
		return 0;

}

bool Eeprom::writeNumber(int address, float data, int amount) {
	uint8_t buff[amount];
	if(amount>1){
		data *= 1000;
	}

	int dd = data;
	for (int a = amount - 1; a >= 0; a--) {
		buff[a] = dd >> (8 * a);
	}

	/* Data to be read from EEPROM */
	int ret_code = Chip_EEPROM_Write(address, buff, amount);

	/* Error checking */
	if (ret_code == IAP_CMD_SUCCESS) {
		return 1;
	} else
		return 0;

}

bool Eeprom::read(int address, uint8_t *data, int amount) {

	int ret_code = Chip_EEPROM_Read(address, data, amount);
	if (ret_code == IAP_CMD_SUCCESS) {
		return true;
	} else {
		return false;
	}
}

/*
 * reads atleast 2 bytes for eeprom, then combines them and forms float
 */
bool Eeprom::readAndCombine(int address, int amount, float *data, int dec) {
	bool ret = 0;
	if (amount > 1) {
		uint8_t buff[amount];
		int ret_code = Chip_EEPROM_Read(address, buff, amount);
		if (ret_code == IAP_CMD_SUCCESS) {
			ret = 1;
		}
		if (ret) {
			int tt = buff[0];
			for (int a = 1; a < amount; a++) {
				tt = (tt << 8) | buff[a];
			}
			*data = tt;
			float exp=10;
			for(int a=1; a< dec; a++){
				exp*=10;
			}
			(*data) = (*data) / exp;
		}
	}
	return ret;

}
