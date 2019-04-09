#ifndef I2C_H
#define I2C_H

#include <inttypes.h>
#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"

#define MT_SLA_ACK							0x18
#define DDR_SPI 							DDRB
#define START 								0x08
#define I2C_TIMEOUT_TIME 					2

extern volatile uint8_t i2c_timeout_ticks;

class i2c{
	public:
		i2c(uart &serialInterface);
		bool WriteData(uint8_t address, uint8_t reg, uint8_t data, bool withData);
		bool ReadRegisteru8(uint8_t address, uint8_t reg, uint8_t *value);
		bool ReadRegisters16LE(uint8_t address, uint8_t reg, int16_t *value);
		bool ReadRegisteru16LE(uint8_t address, uint8_t reg, uint16_t *value);
		bool ReadRegisteru16(uint8_t address, uint8_t reg, uint16_t *value);
		bool ReadRegisterFlow(uint8_t address, uint8_t reg, uint8_t cnt, uint8_t *buffer);

	private:
		uart m_serial;

		uint8_t read_buffer[10];
		void SendNack(void);
		bool didI2cTimeout();
		void do_i2c(uint8_t reg, uint8_t bit);
		void resetTimeout();
		void Init(void);
		bool SendReadSlave(uint8_t address);
		bool SendStopBit(void);
		bool SendStartBit(void);
		bool ReadByte(bool nack, uint8_t *result);
		bool SendByte(uint8_t data);
		bool Write(uint8_t address, uint8_t reg);
		bool WriteDataRaw(uint8_t address, uint8_t reg, uint8_t data, bool withData);
};




#endif