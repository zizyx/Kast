/////////////////////////////////////////////////////////////////////////////////
// PWM
/////////////////////////////////////////////////////////////////////////////////
#include "systemTimer.h"

systemTimer systemTimer::instance = systemTimer();

systemTimer::systemTimer(){
	init();
	DDRB |= (1<<5);         //PB as output
//    PORTB |= (1<<5);         //keep all LEDs off
}

systemTimer *systemTimer::getInstance(){
	return &instance;	
}

void systemTimer::init(){
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	OCR1A =	0xff;
	TIFR1 = 0x00;
	TIMSK1 = 0x00;

	//FAST PWM with OCRA
	// TCCR1A |= (0 << WGM11) | (0 << WGM10);

	//Set clock prescaler to clk no-prescaler
	//	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS11) | (0 << CS12);

	//Output compare register
	OCR1A =	OCR_PRESCALER_10HZ;

	TIMSK1 = 1 << OCIE1A;
}

void systemTimer::enable(){

}

void systemTimer::disable(){

}

ISR(TIMER1_COMPA_vect){
	PINB = (1<<5);
	// OCR1A =	OCR_PRESCALER_10HZ;
}
