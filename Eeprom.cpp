#include "Eeprom.h"

Eeprom::Eeprom(bool a) {
	/* Enable EEPROM clock and reset EEPROM controller */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_EEPROM);
	Chip_SYSCTL_PeriphReset(RESET_EEPROM);
	ptr= (uint8_t *)buffer32;
}

bool Eeprom::write(int address, uint8_t *data, int amount) {

	/* Data to be read from EEPROM */
	int ret_code = Chip_EEPROM_Write(address, data,
			amount);

	/* Error checking */
	if (ret_code == IAP_CMD_SUCCESS) {
		return 1;
	}
	else return 0;

}

bool Eeprom::read(int address, uint8_t *data, int amount) {

	int ret_code = Chip_EEPROM_Read(address, data, amount);
	if (ret_code == IAP_CMD_SUCCESS) {
		return true;
	} else {
		return false;
	}
}
