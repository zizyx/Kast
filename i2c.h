#ifndef I2C_H
#define I2C_H

#include <inttypes.h>
#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"

#define MT_SLA_ACK							0x18
#define DDR_SPI 							DDRB
#define START 								0x08
#define I2C_TIMEOUT_TIME 						2

extern volatile uint8_t i2c_timeout_ticks;

class i2c{
	public:
		i2c(uart &serialInterface);
		void WriteData(uint8_t address, uint8_t reg, uint8_t data, bool withData);
		uint8_t ReadRegisteru8(uint8_t address, uint8_t reg);
		uint16_t ReadRegisteru16(uint8_t address, uint8_t reg);
		uint16_t ReadRegisteru16LE(uint8_t address, uint8_t reg);
		int16_t ReadRegisters16(uint8_t address, uint8_t reg);
		int16_t ReadRegisters16LE(uint8_t address, uint8_t reg);
		void ReadRegisterFlow(uint8_t address, uint8_t reg, uint8_t cnt, uint8_t *buffer);

	private:
		uart m_serial;

		void Init(void);
		bool SendByte(uint8_t data);
		void SendStopBit(void);
		bool SendStartBit(void);
		uint8_t read_buffer[10];
		void SendNack(void);
		void SendReadSlave(uint8_t address);
		void Write(uint8_t address, uint8_t reg);
		void WriteDataRaw(uint8_t address, uint8_t reg, uint8_t data, bool withData);
		uint8_t ReadByte(bool nack);
		bool didI2cTimeout();
		void resetTimeout();
};

#endif