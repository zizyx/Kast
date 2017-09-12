#include <util/delay.h>
#include <String.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#include "BMP_280.h"
#include "DS_3231.h"
#include "uart.h"
#include "adc.h"
#include "pdm.h"
#include "climateControl.h"
#include "board.h"

// extern "C" void __cxa_pure_virtual()
// {
// 	while (1);
// }

int main(){

	// pdm *Pdm;
	// DS_3231 *clock;
	uart *debug;
	// adc *Adc;
	climateControl cctl;

	cctl = climateControl();

	// char string[81] = {"e\n\0"};

	debug = uart::getInstance();
	// Adc = adc::getInstance();
	// clock = DS_3231::getInstance();
	// Pdm = pdm::getInstance();

	// testing uart
	// Pdm->setupFanPin(FAN_PIN); // PIN 11
	// debug->TransmitString((char *)"Hallo!\n");

	// datetime_t new_dt = datetime_t(22, 22, 2, 2, 20, 2, 2022);
	// clock->setTime(new_dt);
	
	while(1){

		// sprintf(string, "pressure is %ld, temp is %d.\n", baro_inside.ReadPressure(), baro_inside.ReadTemperatureRound());
		// debug->TransmitString(string);
		// _delay_ms(1000); 

		debug->checkBuffer(&cctl);
		// debug->TransmitString(clock->getCurrentTime().toString());
		// debug->TransmitString("\n\0");

		// //Starts conversion, result is done after 100ms?
		// Adc->setChannel(CHANNEL_ZERO);
		// Adc->startFirstConversion();
		// _delay_ms(100); 
		// sprintf(string, "Adc result is %d.\n", Adc->readAdc());	
		// debug->TransmitString(string);

		_delay_ms(1000); 
	}

	return 0;
}
