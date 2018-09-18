/////////////////////////////////////////////////////////////////////////////////
// I2C
/////////////////////////////////////////////////////////////////////////////////

#include "i2c.h"
#include <avr/interrupt.h>

volatile uint8_t i2c_timeout_ticks = 0;

i2c::i2c(uart &serialInterface) : 
	m_serial(serialInterface)
{
	Init();
}


void i2c::resetTimeout() {
	cli();
	i2c_timeout_ticks = 0;
	sei();
}

bool i2c::didI2cTimeout() {
	if (i2c_timeout_ticks >= I2C_TIMEOUT_TIME) {
		m_serial.print("I2C Error: Did timeout\n"); //TODO DEBUG kan uit, error print?
		return true;
	}
	return false;
}

void i2c::SendNack(void){
	TWCR = (1<<TWINT) | (1<<TWEN); /* send NAK this time (stop command?) */
	resetTimeout();
	while((!didI2cTimeout()) && (TWCR & (1<<TWINT)) == 0); /* wait for nack */
}

bool i2c::ReadRegisteru8(uint8_t address, uint8_t reg, uint8_t *value) {
	bool result;

	Write(address, reg);
	SendReadSlave(address);
	result = ReadByte(true, value);
	if (result == false)
		return false;

	result = SendStopBit();
	if (result == false)
		return false;

	return true;
}

bool i2c::ReadRegisters16LE(uint8_t address, uint8_t reg, int16_t *value) {
	return ReadRegisteru16LE(address, reg, (uint16_t *)value);
}


bool i2c::ReadRegisteru16LE(uint8_t address, uint8_t reg, uint16_t *value) {
	bool result;

	result = ReadRegisteru16(address, reg, value);
	*value = (((*value) >> 8) | ((*value) << 8));

	return result;
}


bool i2c::ReadRegisteru16(uint8_t address, uint8_t reg, uint16_t *value){
	bool result;
	uint8_t msb, lsb;
	
	Write(address, reg);
	SendReadSlave(address);
	result = (ReadByte(false, &msb) << 8);
	if (result == false)
		return false;

	result = ReadByte(true, &lsb);
	if (result == false)
		return false;

	if (SendStopBit() == false)
		return false;

	*value = msb;
	*value = ((*value) << 8);
	*value |= lsb;

	return true;
}

bool i2c::ReadRegisterFlow(uint8_t address, uint8_t reg, uint8_t cnt, uint8_t *buffer) {
	uint8_t readAddress;

	if (SendStartBit() == false)
		return false;

	// SLAVE ADRESSS PLUS WRITE
	readAddress = ((address << 1) & 0xFE);
	if (SendByte(readAddress) == false) //inc ack
		return false;

	//Word addre / reg
	if (SendByte(reg) == false) //inc ack
		return false;

	if (SendStartBit() == false) //Sr
		return false;

	//Slave addr plus read
	readAddress = ((address << 1) | 0x01);
	if (SendByte(readAddress) == false) //inc ack
		return false;

	// debug->print("buffer = ");
	uint8_t max = 6;
	for (uint8_t i = 0; i <= max; i++) {
		if (i < max) {
			if (ReadByte(false, &buffer[i]) == false)
				return false;
			// buffer[i] = ReadByte(false);
		} else {
			if (ReadByte(true, &buffer[i]) == false) //After last byte send nack
				return false;
			// buffer[i] = ReadByte(true); //After last byte send nack
		}

	//	debug->Transmit(buffer[i]);
	}

	if (SendStopBit() == false)
		return false;

	return true;
}

void i2c::Init(void){
	// set timer prescaler
	TWBR = 72;
	TWSR = 0b00000000;

	// enable I2C hardware
	TWCR = (1<<TWEN);
}

bool i2c::SendReadSlave(uint8_t address){

	if (SendStartBit() == false)
		return false;

	// SLAVE ADRESSS plus READ command
	TWDR = (address<<1) | 0x01; //0x76 met SDO = GND, 0x77 met SDO = VCC
	TWCR = (1<<TWINT) | (1<<TWEN);

	// WAIT FOR ADDRESS TO BE SENT
	resetTimeout();
	while((!didI2cTimeout()) && !(TWCR & (1<<TWINT)));

	if (didI2cTimeout())
		return false;

	return true;
}

bool i2c::SendStopBit(void){
	//Start sending stop bit
	TWCR = (1<<TWINT)| (1<<TWEN)|(1<<TWSTO);

	//Wait for stop bit to be sent //IF BARO is broken this can be teh problem
	resetTimeout();
	while((!didI2cTimeout()) && (TWCR & (1<<TWSTO)) ==  0x00);

	if (didI2cTimeout())
		return false;

	return true;

}

bool i2c::SendStartBit(void){
	//SEND START CONDITION 
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	// WAIT FOR START TO BE SENT
	resetTimeout();
	while((!didI2cTimeout()) && !(TWCR & (1<<TWINT))){
		// debug->print("Start has been sent (I2C)\n");
	}

	// //CHECK IF START FAILED
	// if ( (TWSR == 0x08) ){
	// 	debug->print("Start sent :) (I2C)\n");
	// 	debug->Transmit(TWSR);
	// } else if ( (TWSR == 0x10) ){
	// 	debug->print("Repeated Start sent :) (I2C)\n");
	// 	debug->Transmit(TWSR);
	// } else if ( (TWSR == 0xF8) ){
	// 	debug->print("No bus info (I2C)\n");
	// 	debug->Transmit(TWSR);
	// } else {
	// 	debug->print("Something has failed (I2C)\n");
	// 	debug->Transmit(TWSR);
	// }

	if (didI2cTimeout())
		return false;

	return true;

}

bool i2c::ReadByte(bool nack, uint8_t *result){

	if(nack == true){
		TWCR = (1<<TWINT) | (1<<TWEN); /* send NAK this time */
	} else {
		TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);		/* clear int to start recieving data (start command?)*/
	}

	resetTimeout();
	while((!didI2cTimeout()) && (TWCR & (1<<TWINT)) == 0); /* wait for data */

	if (didI2cTimeout())
		return false;

	//return recieved data
	*result = TWDR;
	return true;	
}

bool i2c::SendByte(uint8_t data){
	// DATA TO BE SENT
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// WAIT FOR DATA TO BE SENT AND RECEIVE AN ACKNOWLEDGE
	resetTimeout();
	while((!didI2cTimeout()) && !(TWCR & (1<<TWINT)));

	if (didI2cTimeout())
		return false;

	return true;
}

bool i2c::Write(uint8_t address, uint8_t reg) {
	return WriteData(address, reg, 0x00, false);
}	

bool i2c::WriteData(uint8_t address, uint8_t reg, uint8_t data, bool withData) {

	if (SendStartBit() == false)
		return false;

	if (WriteDataRaw(address, reg, data, withData) == false)
		return false;

	if (SendStopBit() == false)
		return false;

	return true;	
}


bool i2c::WriteDataRaw(uint8_t address, uint8_t reg, uint8_t data, bool withData) {
	// SLAVE ADRESSS PLUS WRITE
	uint8_t readAddress = ((address << 1) & 0xFE);
	
	if (SendByte(readAddress) == false)
		return false;

	//CHECK IF SENDING ADDRESS FAILED
	resetTimeout();
	while((!didI2cTimeout()) && (TWSR & 0xF8) != MT_SLA_ACK){
		// PRINT_STR(debug, "No ack received (I2C) implement timeout\n");
	}
		
	if (SendByte(reg) == false)
		return false;

	if(withData)
		if (SendByte(data) == false)
			return false;

	return true;
}
