#ifndef NVM_H
#define NVM_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "uart.h"

#define NVM_BASE						0x0000
#define NVM_SIZE 						1024

#define	EEPROM_VALID_VAL					0xAF
#define	EEPROM_VALID_OFFSET					NVM_BASE
#define EEPROM_ERASE_VAL					0x00
#define NVM_TIMEOUT_TIME					20	

extern volatile uint8_t nvm_timeout_ticks;

class nvm {
	public:
		nvm(uart &serialInterface);
		uint8_t nvmRead(uint16_t address);
		uint8_t calcCrc(uint8_t *data, uint16_t len);
		void nvmWrite(uint16_t address, uint8_t data);
		void nvmWriteBlock(uint16_t address, uint8_t *data, uint8_t bufferSize);
		void nvmReadBlock(uint16_t address, uint8_t *buffer, uint8_t bufferSize);

	private:
		uart m_serial; 
		bool didNvmTimeout();
		void resetTimeout();
};

#endif