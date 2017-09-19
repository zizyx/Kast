#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "climateControl.h"
#include "board.h"

int main(){

	uart *debug;
	climateControl cctl;
	cctl = climateControl();
	debug = uart::getInstance();


	while(1){
		debug->checkBuffer(&cctl);
		// //Starts conversion, result is done after 100ms?
		// Adc->setChannel(CHANNEL_ZERO);
		// Adc->startFirstConversion();
		// _delay_ms(1000); 
		// sprintf(string, "Adc result is %d.\n", Adc->readAdc());	
		// debug->TransmitString(string);
		// _delay_ms(1);
	}
	return 0;
}
