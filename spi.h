#ifndef SPI_H
#define SPI_H

#define DDR_SPI 	DDRB

#include <avr/io.h>

class spi{
	public:
		spi();
		void MasterInit(void);
		void MasterTransmit(void);
	private:
};

#endif