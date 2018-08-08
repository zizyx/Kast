#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "climateControl.h"
#include "board.h"
#include "systemTimer.h"
#include "alarm.h"
#include "nvm.h"
#include "DS_3231.h"

int main(){
	/* The order of m_clock, m_alarm and m_timer need to be created in this order. */
	DS_3231 m_clock;
	alarm m_alarm(m_clock);
	systemTimer m_timer(m_clock, m_alarm);

	uart m_debug;
	i2c m_twi(m_debug);
	climateControl m_cctl(m_twi, m_debug, m_clock);

	// DS_3231 *clock = DS_3231::getInstance();
	// char string[81];

	// nvm *nvm = nvm::getInstance();

	while(1){
//		m_debug.callback = &climateControl::handleCmd;

		m_debug.checkBuffer(m_cctl, &climateControl::handleCmd);
//		m_debug.checkBuffer();
		// PRINT_STR("TEST");
		m_timer.oncePerSecondTimer();
		// m_timer.oncePerTenSecondsTimer();

		///////////////////////////////////////////////////////////
		// EEPROM TEST
		// char string[81] = {"e\n\0"};
		// uint8_t array[80];
		// uint8_t array2[80];		
		// for (uint8_t i = 0; i < sizeof(array); ++i){
		// 	array[i] = i;
		// }
		
		// nvm->nvmWriteBlock(5, array, sizeof(array));
		// nvm->nvmReadBlock(0, array2, sizeof(array2));
		
		// for (uint8_t j = 0; j < 10; j++)
		// {
		// 	sprintf(string, "Memory is %d.\n", array2[j]);	
		// 	m_debug->print(string);
		// }
		// _delay_ms(1000);
		///////////////////////////////////////////////////////////


		// cctl->checkClimate();
		// Starts conversion, result is done after 100ms?
		// Adc->setChannel(CHANNEL_ZERO);
		// Adc->startFirstConversion();
		// _delay_ms(1000); 
		// sprintf(string, "Adc result is %d.\n", Adc->readAdc());	
		// m_debug->TransmitString(string);
		// _delay_ms(1);

		// sprintf(string, "Datetime: %s\n", clock->getCurrentTime().toString());
		// PRINT_STR(string);
		_delay_ms(10);
	}
	return 0;
}


