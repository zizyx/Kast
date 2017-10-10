#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "climateControl.h"
#include "board.h"
#include "systemTimer.h"
#include "alarm.h"

int main(){

	alarm alarms;
	climateControl cctl;
	uart *debug = uart::getInstance();
	systemTimer *timer = systemTimer::getInstance();

	while(1){
		debug->checkBuffer(&cctl);

		timer->oncePerSecondTimer();
		timer->oncePerTenSecondsTimer();

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
