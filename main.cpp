#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "climateControl.h"
#include "board.h"
#include "systemTimer.h"
#include "alarm.h"
#include "nvm.h"

int main(){
	climateControl cctl;
	uart *debug = uart::getInstance();
	systemTimer *timer = systemTimer::getInstance();
	// nvm *nvm = nvm::getInstance();

	while(1){

		debug->checkBuffer(&cctl);

		timer->oncePerSecondTimer();
		timer->oncePerTenSecondsTimer();

		///////////////////////////////////////////////////////////
		// EEPROM TEST
		// uint8_t array[80];
		// uint8_t array2[80];		
		// for (i = 0; i < sizeof(array); ++i){
		// 	array[i] = i;
		// }
		//
		// nvm->nvmWriteBlock(5, array, sizeof(array));
		// nvm->nvmReadBlock(0, array2, sizeof(array2));
		//
		// for (int j = 0; j < 10; j++)
		// {
		// 	sprintf(string, "Memory is %d.\n", array2[j]);	
		// 	debug->TransmitString(string);
		// }
		// _delay_ms(1000);
		///////////////////////////////////////////////////////////


		// cctl->checkClimate();
		// Starts conversion, result is done after 100ms?
		// Adc->setChannel(CHANNEL_ZERO);
		// Adc->startFirstConversion();
		// _delay_ms(1000); 
		// sprintf(string, "Adc result is %d.\n", Adc->readAdc());	
		// debug->TransmitString(string);
		// _delay_ms(1);
	}
	return 0;
}


