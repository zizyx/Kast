#include "spi.h"

// #define START_REP 	0x10
// #define MT_DATA_ACK 	0x28
// #define MR_SLA_ACK  	0x40
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// SPI
/////////////////////////////////////////////////////////////////////////////////

spi::spi(){
	MasterInit();
}
void spi::MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = (1<<2)|(1<<3)|(1<<5);

	PORTB = 0b00000100;
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<6)|(1<<4)|(1<<0);
}

void spi::MasterTransmit(void)
{
	PORTB &= ~(1<<2);

	/* Start transmission */
	SPDR = 0b11011001;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<7)));
	PORTB |= (1<<2);
}
/////////////////////////////////////////////////////////////////////////////////