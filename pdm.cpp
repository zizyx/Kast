/////////////////////////////////////////////////////////////////////////////////
// PWM
/////////////////////////////////////////////////////////////////////////////////
#include "pdm.h"

pdm instance = pdm();

pdm::pdm(){

}

pdm *pdm::getInstance(){
	return &instance;	
}

//Choose any pin from port b
void pdm::setupFanPin(uint8_t pinNr){
	DDRB |= OUTPUT << pinNr; //Direction
	PORTB |= (PULL_UP << pinNr);
	// PINB |= HIGH << pinNr;
}


void pdm::setupPinTimer(){
	TCCR2A = 0x00;
	TCCR2B = 0x00;
	OCR2A =	0x00;
	TIFR2 = 0x00;

//Depends on WGM22 to be set.!!!!!
//Clear OC2A on Compare Match, set OC2A at BOTTOM,
	TCCR2A |= (1 << COM2A1); 

//FAST PWM with OCRA
	TCCR2A |= (1 << WGM22) | (1 << WGM21) | (1 << WGM20);

//Set clock prescaler to clk no-prescaler
//	TCCR2B |= (1 << CS20);
	TCCR2B |= (1 << CS20) | (1 << CS21) |(1 << CS22);

//Output compare register
	OCR2A =	255;
}

void pdm::setPdm(uint8_t pdm){
	//Output compare register
	OCR2A =	pdm;
}

// ISR(TIMER2_OVF_vect){
// 	OCR2A =	10;
// 	// if(counter_direction) {
// 	// 	OCR2A =	2;
// 	// } else {
// 	// 	OCR2A = 2;
// 	// }
// }
