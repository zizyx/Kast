/////////////////////////////////////////////////////////////////////////////////
// I2C
/////////////////////////////////////////////////////////////////////////////////

#include "i2c.h"
#include <avr/interrupt.h>
#include "uart.h"

volatile uint8_t i2c_timeout_ticks = 0;

i2c::i2c() {
	Init();
}


void i2c::resetTimeout() {
	cli();
	i2c_timeout_ticks = 0;
	sei();
}

bool i2c::didI2cTimeout() {
	if (i2c_timeout_ticks >= I2C_TIMEOUT_TIME) {
		// DEBUG_STR("I2C Error: Did timeout\n"); //TODO DEBUG kan uit, error print?
		return true;
	}
	return false;
}

void i2c::SendNack(void){
	TWCR = (1<<TWINT) | (1<<TWEN); /* send NAK this time (stop command?) */
	resetTimeout();
	while((!didI2cTimeout()) && (TWCR & (1<<TWINT)) == 0); /* wait for nack */
}

uint8_t i2c::ReadRegisteru8(uint8_t address, uint8_t reg){
	Write(address, reg);
	SendReadSlave(address);
	uint8_t value = ReadByte(true);
	SendStopBit();
	return value;
}

int16_t i2c::ReadRegisters16LE(uint8_t address, uint8_t reg){
	return (int16_t)ReadRegisteru16LE(address, reg);
}


uint16_t i2c::ReadRegisteru16LE(uint8_t address, uint8_t reg){
	uint16_t value;

	value = ReadRegisteru16(address, reg);

	value = ((value >> 8) | (value << 8));

	return value;
}


uint16_t i2c::ReadRegisteru16(uint8_t address, uint8_t reg){
	uint16_t value;
	
	Write(address, reg);
	SendReadSlave(address);
	value = (ReadByte(false)<<8);
	value |= ReadByte(true);
	SendStopBit();

	return value;
}

void i2c::ReadRegisterFlow(uint8_t address, uint8_t reg, uint8_t cnt, uint8_t *buffer) {
	// uint8_t test[7] = {1,2,3,4,5,6,7};


	uint8_t readAddress;

	SendStartBit();

	// SLAVE ADRESSS PLUS WRITE
	readAddress = ((address << 1) & 0xFE);
	SendByte(readAddress); //inc ack

	//Word addre / reg
	SendByte(reg); //inc ack

	SendStartBit(); //Sr

	//Slave addr plus read
	readAddress = ((address << 1) | 0x01);
	SendByte(readAddress); //inc ack

	// debug->print("buffer = ");
	uint8_t max = 6;
	for(uint8_t i = 0; i <= max; i++){

		if (i < max)
			buffer[i] = ReadByte(false);
		else
			buffer[i] = ReadByte(true); //After last byte send nack

	//	debug->Transmit(buffer[i]);
	}

	
	// uint8_t i = 0;
	// buffer[i++] = ReadByte(false);
	// buffer[i++] = ReadByte(false);
	// buffer[i++] = ReadByte(false);
	// buffer[i++] = ReadByte(false);
	// buffer[i++] = ReadByte(false);
	// buffer[i++] = ReadByte(false); 
	// buffer[i++] = ReadByte(true); //After last byte send nack

//	TWCR = (1<<TWINT) | (1<<TWEN); /* send NAK this time */

	SendStopBit();

	// memcpy(buffer, test, 7);
//	return buffer;
}

void i2c::Init(void){
	// set timer prescaler
	TWBR = 72;
	TWSR = 0b00000000;

	// enable I2C hardware
	TWCR = (1<<TWEN);
}

void i2c::SendReadSlave(uint8_t address){
	SendStartBit();

	// SLAVE ADRESSS plus READ command
	TWDR = (address<<1) | 0x01; //0x76 met SDO = GND, 0x77 met SDO = VCC
	TWCR = (1<<TWINT) | (1<<TWEN);

	// WAIT FOR ADDRESS TO BE SENT
	resetTimeout();
	while((!didI2cTimeout()) && !(TWCR & (1<<TWINT)));
}

void i2c::SendStopBit(void){
	//Start sending stop bit
	TWCR = (1<<TWINT)| (1<<TWEN)|(1<<TWSTO);

	//Wait for stop bit to be sent //IF BARO is broken this can be teh problem
	resetTimeout();
	while((!didI2cTimeout()) && (TWCR & (1<<TWSTO)) ==  0x00);
}

void i2c::SendStartBit(void){
	//SEND START CONDITION 
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	// WAIT FOR START TO BE SENT
	resetTimeout();
	while((!didI2cTimeout()) && !(TWCR & (1<<TWINT))){
		// debug->print("Start has been sent (I2C)\n");
	}

	//CHECK IF START FAILED
	if ( (TWSR == 0x08) ){
		// debug->print("Start sent :) (I2C)\n");
		// debug->Transmit(TWSR);
	} else if ( (TWSR == 0x10) ){
		// debug->print("Repeated Start sent :) (I2C)\n");
		// debug->Transmit(TWSR);
	} else if ( (TWSR == 0xF8) ){
		// debug->print("No bus info (I2C)\n");
		// debug->Transmit(TWSR);
	} else {
		// debug->print("Something has failed (I2C)\n");
		// debug->Transmit(TWSR);
	}

}

uint8_t i2c::ReadByte(bool nack){
	if(nack == true){
		TWCR = (1<<TWINT) | (1<<TWEN); /* send NAK this time */
	} else {
		TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);		/* clear int to start recieving data (start command?)*/
	}

	i2c_timeout_ticks = 0;
	while((!didI2cTimeout()) && (TWCR & (1<<TWINT)) == 0); /* wait for data */

	//return recieved data
	return TWDR;	
}

void i2c::SendByte(uint8_t data){
		// DATA TO BE SENT
		TWDR = data;
		TWCR = (1<<TWINT) | (1<<TWEN);

		// WAIT FOR DATA TO BE SENT AND RECEIVE AN ACKNOWLEDGE
		cli();
		i2c_timeout_ticks = 0;
		sei();
		while((!didI2cTimeout()) && !(TWCR & (1<<TWINT)));
}

void i2c::Write(uint8_t address, uint8_t reg){
	WriteData(address, reg, 0x00, false);
}	

void i2c::WriteData(uint8_t address, uint8_t reg, uint8_t data, bool withData){
	SendStartBit();

	WriteDataRaw(address, reg, data, withData);

	SendStopBit();	
}


void i2c::WriteDataRaw(uint8_t address, uint8_t reg, uint8_t data, bool withData){
	// SLAVE ADRESSS PLUS WRITE
	uint8_t readAddress = ((address << 1) & 0xFE);
	SendByte(readAddress);

	//CHECK IF SENDING ADDRESS FAILED
	resetTimeout();
	while((!didI2cTimeout()) && (TWSR & 0xF8) != MT_SLA_ACK){
		// PRINT_STR(debug, "No ack received (I2C) implement timeout\n");
	}
		
	SendByte(reg);

	if(withData) {
		SendByte(data);
	}
}
