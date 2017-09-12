#ifndef I2C_H
#define I2C_H

#define MT_SLA_ACK							 0x18
#define DDR_SPI 							 DDRB
#define START 								 0x08

#include <inttypes.h>
#include <avr/io.h>
#include <stdlib.h>

class i2c{
	public:
		i2c(void);
		static i2c *getInstance();
		void WriteData(uint8_t address, uint8_t reg, uint8_t data, bool withData);
		uint8_t ReadRegisteru8(uint8_t address, uint8_t reg);
		uint16_t ReadRegisteru16(uint8_t address, uint8_t reg);
		uint16_t ReadRegisteru16LE(uint8_t address, uint8_t reg);
		int16_t ReadRegisters16(uint8_t address, uint8_t reg);
		int16_t ReadRegisters16LE(uint8_t address, uint8_t reg);
		void ReadRegisterFlow(uint8_t address, uint8_t reg, uint8_t cnt, uint8_t *buffer);

	private:
		static i2c instance;
		void Init(void);
		void SendByte(uint8_t data);
		void SendStopBit(void);
		void SendStartBit(void);
		uint8_t read_buffer[10];
		void SendNack(void);
		void SendReadSlave(uint8_t address);
		void Write(uint8_t address, uint8_t reg);
		void WriteDataRaw(uint8_t address, uint8_t reg, uint8_t data, bool withData);
		uint8_t ReadByte(bool nack);
};

#endif