#include "nvm.h"
#include "uart.h"

volatile uint8_t nvm_timeout_ticks = 0;

nvm::nvm() {
	if (nvmRead(EEPROM_VALID_OFFSET) != EEPROM_VALID_VAL) {
		// DEBUG_STR("Initialising EEPROM\n");

		//EEPROM is not valid.
		nvmWrite(EEPROM_VALID_OFFSET, EEPROM_VALID_VAL);
	}
}

void nvm::resetTimeout() {
	// make sure to disable and enable interrupts before and after
	nvm_timeout_ticks = 0;
}

bool nvm::didNvmTimeout() {
	if (nvm_timeout_ticks >= NVM_TIMEOUT_TIME) {
		// DEBUG_STR("NVM Error: Did timeout\n"); 
		return true;
	}
	return false;
}

uint8_t nvm::calcCrc(uint8_t *data, uint16_t len) {
	uint16_t crc;

	crc = 0x00;
	
	for (uint16_t j = len, data_idx = 0; j > 0; j--, data_idx++) {
		crc ^= (data[data_idx] << 8);

		for (uint8_t i = 8; i; i--) {
			if (crc & 0x8000) {
				crc ^= (0x1070 << 3);
			}
			crc <<= 1;
		}
	}
	return (crc >>= 8);
}

void nvm::nvmWrite(uint16_t address, uint8_t data) {
	// Writing EEPROM should not be interrupted
	cli();
	resetTimeout();
	/* Wait for completion of previous write */
	while(!didNvmTimeout() && (EECR & (1<<EEPE)));
	/* Set up address and Data Registers */
	EEAR = address;
	EEDR = data;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start nvm write by setting EEPE */
	EECR |= (1<<EEPE);
	sei();
}

uint8_t nvm::nvmRead(uint16_t address) {
	// Reading EEPROM should not be interrupted
	cli();
	resetTimeout();
	/* Wait for completion of previous write */
	while(!didNvmTimeout() && (EECR & (1<<EEPE)));
	/* Set up address register */
	EEAR = address;
	/* Start nvm read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	sei();
	return EEDR;
}

void nvm::nvmWriteBlock(uint16_t address, uint8_t *data, uint8_t bufferSize) {
	uint16_t idx = 0;
	for (uint16_t i = address; i < bufferSize; i++, idx++)
		nvmWrite(i, data[idx]);
}

void nvm::nvmReadBlock(uint16_t address, uint8_t *buffer, uint8_t bufferSize) {
	uint16_t idx = 0;
	for (uint16_t i = address; i < bufferSize; i++, idx++)
		buffer[i] = nvmRead(idx);
}

// void nvm::nvmEraseBlock(uint16_t address, uint8_t size) {
// 	for (uint16_t i = address; i < bufferSize; i++, idx++)
// 		nvmWrite(i, EEPROM_ERASE_VAL);
// }
