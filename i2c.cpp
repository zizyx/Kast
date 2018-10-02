/////////////////////////////////////////////////////////////////////////////////
// I2C
/////////////////////////////////////////////////////////////////////////////////

#include "i2c.h"
#include <avr/interrupt.h>
#include "helper.h" 

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

void i2c::do_i2c(uint8_t reg, uint8_t bit) {	
	resetTimeout();
	while((!didI2cTimeout()) && !(reg & (1<<bit)));
}


void i2c::SendNack(void){
	TWCR = (1<<TWINT) | (1<<TWEN); /* send NAK this time (stop command?) */
	do_i2c(TWCR, TWINT); /* wait for nack */
}

bool i2c::ReadRegisteru8(uint8_t address, uint8_t reg, uint8_t *value) {
	Write(address, reg);
	SendReadSlave(address);
	return_if_false(ReadByte(true, value));
	return_if_false(SendStopBit());

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
	uint8_t msb, lsb;
	
	Write(address, reg);
	SendReadSlave(address);
	return_if_false((ReadByte(false, &msb) << 8));
	return_if_false(ReadByte(true, &lsb));
	return_if_false(SendStopBit());

	*value = to_uint16_t(msb, lsb);

	return true;
}

bool i2c::ReadRegisterFlow(uint8_t address, uint8_t reg, uint8_t cnt, uint8_t *buffer) {
	uint8_t max = 6;
	uint8_t readAddress;

	return_if_false(SendStartBit());

	// SLAVE ADRESSS PLUS WRITE
	readAddress = ((address << 1) & 0xFE);
	return_if_false(SendByte(readAddress)); //inc ack

	//Word addre / reg
	return_if_false(SendByte(reg)); //inc ack

	return_if_false(SendStartBit()); //Sr

	//Slave addr plus read
	readAddress = ((address << 1) | 0x01);
	return_if_false(SendByte(readAddress)); //inc ack

	// debug->print("buffer = ");
	for (uint8_t i = 0; i <= max; i++) {
		if (i < max) {
			return_if_false(ReadByte(false, &buffer[i]));
		} else {
			return_if_false(ReadByte(true, &buffer[i])); //After last byte send nack
		}
	//	debug->Transmit(buffer[i]);
	}
	return_if_false(SendStopBit());

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
	return_if_false(SendStartBit());

	// SLAVE ADRESSS plus READ command
	TWDR = (address<<1) | 0x01; //0x76 met SDO = GND, 0x77 met SDO = VCC
	TWCR = (1<<TWINT) | (1<<TWEN);

	// WAIT FOR ADDRESS TO BE SENT
	do_i2c(TWCR, TWINT);

	return_if_true(didI2cTimeout());
	
	return true;
}

bool i2c::SendStopBit(void){
	//Start sending stop bit
	TWCR = (1<<TWINT)| (1<<TWEN)|(1<<TWSTO);

	//Wait for stop bit to be sent //IF BARO is broken this can be teh problem
	do_i2c(TWCR, TWSTO);

	return_if_true(didI2cTimeout());
	
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

	return_if_true(didI2cTimeout());
	
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

	return_if_true(didI2cTimeout());
	
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

	return_if_true(didI2cTimeout());

	return true;
}

bool i2c::Write(uint8_t address, uint8_t reg) {
	return WriteData(address, reg, 0x00, false);
}	

bool i2c::WriteData(uint8_t address, uint8_t reg, uint8_t data, bool withData) {

	return_if_false(SendStartBit());
	return_if_false(WriteDataRaw(address, reg, data, withData));
	return_if_false(SendStopBit());

	return true;	
}


bool i2c::WriteDataRaw(uint8_t address, uint8_t reg, uint8_t data, bool withData) {
	// SLAVE ADRESSS PLUS WRITE
	uint8_t readAddress = ((address << 1) & 0xFE);
	
	return_if_false(SendByte(readAddress));

	//CHECK IF SENDING ADDRESS FAILED
	resetTimeout();
	while((!didI2cTimeout()) && (TWSR & 0xF8) != MT_SLA_ACK){
		// PRINT_STR(debug, "No ack received (I2C) implement timeout\n");
	}
		
	return_if_false(SendByte(reg))

	if(withData)
		return_if_false(SendByte(data))

	return true;
}
