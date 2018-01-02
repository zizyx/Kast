#include "nvm.h"

nvm nvm::instance = nvm();

nvm::nvm() {
}

nvm *nvm::getInstance() {
	return &instance;
}

uint8_t calcCrc(uint8_t *data, uint16_t len) {
	uint8_t crc;

	crc = 0x00;

	for (uint16_t i = 0; i < len; i++) {
		crc ^= data[0];

		for (uint8_t j = 0; j < 8; j++) {
			if (crc & 0x01) {
			 	crc = (crc >> 1) ^ 0xA001;
			} else {
				crc = (crc >> 1);
			}
		}
	}

	return crc;
}

void nvm::nvmWrite(uint16_t address, uint8_t data) {
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address and Data Registers */
	EEAR = address;
	EEDR = data;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start nvm write by setting EEPE */
	EECR |= (1<<EEPE);
}

uint8_t nvm::nvmRead(uint16_t address) {
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address register */
	EEAR = address;
	/* Start nvm read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
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
