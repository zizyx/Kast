#include <util/delay.h>
// #include "BMP_280.h"
// #include "DS_3231.h"
// #include "uart.h"
// #include "adc.h"
#include <string.h>
#include <stdio.h>
// #include "pdm.h"
#include <avr/interrupt.h>
// #include "climateControl.h"
// #include "board.h"

//TO DO:
// PDM
extern "C" void __cxa_pure_virtual()
{
	while (1);
}

int main() {

	// pdm *Pdm;
	// BMP_280 baro_inside(0x76);
	// BMP_280 baro_outside(0x77);
	// DS_3231 *clock;
	// uart *debug;
	// adc *Adc;
	// climateControl *Climate;

	// char string[81] = {"e\n\0"};

	// debug = uart::getInstance();
	// Adc = adc::getInstance();
	// clock = DS_3231::getInstance();
	// Pdm = pdm::getInstance();
	// Climate = climateControl::getInstance();

	// // testing uart
	// Pdm->setupFanPin(FAN_PIN); // PIN 11
	// Pdm->setupPinTimer();
	// Pdm->setPdm(255);
	// debug->TransmitString((char *)"Hallo!\n");

    // // Timers
    // alarm           m_alarm(m_clock);
    // systemTimer     m_timer(m_clock, m_alarm);

    // // nvm
    // nvm             m_storage;

    // Climate control
    // climateControl  m_cctl(m_baro, m_clock, m_storage);

	// debug->checkBuffer();
	// debug->TransmitString(clock->getCurrentTime().toString());
	// debug->TransmitString("\n\0");

    while(1){
        ///////////////////////////////////////////////////////////
        // EEPROM TEST
        // char string[81] = {"e\n\0"};
        // uint8_t array[80];
        // uint8_t array2[80];      
        // for (uint8_t i = 0; i < sizeof(array); ++i){
        //  array[i] = i;
        // }
        
        // nvm->nvmWriteBlock(5, array, sizeof(array));
        // nvm->nvmReadBlock(0, array2, sizeof(array2));
        
        // for (uint8_t j = 0; j < 10; j++)
        // {
        //  sprintf(string, "Memory is %d.\n", array2[j]);  
        //  m_debug->print(string);
        // }
        // _delay_ms(1000);
        ///////////////////////////////////////////////////////////
		// _delay_ms(1000); 
    }
    return 0;
}


