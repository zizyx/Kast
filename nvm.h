#ifndef NVM_H
#define NVM_H

#define NVM_SIZE 1024

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

struct nvm_record {
	uint8_t data_len;
};

class nvm {
	public:
		static nvm *getInstance();
		uint8_t nvmRead(uint16_t address);
		void nvmWrite(uint16_t address, uint8_t data);
		void nvmWriteBlock(uint16_t address, uint8_t *data, uint8_t bufferSize);
		void nvmReadBlock(uint16_t address, uint8_t *buffer, uint8_t bufferSize);
		uint8_t calcCrc(uint8_t *data, uint16_t len);
	private:
		nvm();
		static nvm instance;
};

#endif