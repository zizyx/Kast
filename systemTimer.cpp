/////////////////////////////////////////////////////////////////////////////////
// systemTimer
/////////////////////////////////////////////////////////////////////////////////
#include "systemTimer.h"
#include "i2c.h"
#include "nvm.h"

systemTimer systemTimer::instance = systemTimer();
volatile int timerOneSecTicks;
volatile int timerTenSecTicks;

systemTimer::systemTimer()
: clock(DS_3231::getInstance())
{
	init();
	DDRB |= (1<<5);         //PB as output
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

	//Set clock prescaler to clk no-prescaler
	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS11) | (0 << CS12);

	//Output compare register
	OCR1A =	OCR_PRESCALER_10HZ;

	TIMSK1 = 1 << OCIE1A;
}

void systemTimer::enable(){

}

void systemTimer::disable(){

}

void systemTimer::oncePerTenSecondsTimer(){
	if(timerTenSecTicks >= 100){
		cli();	
		timerTenSecTicks = 0;
		timerOneSecTicks = 0;
		sei();
		alarms.addNewAlarm((clock->getCurrentTime() + datetime_t(3,0,0,0,0,0,0)));
		alarms.checkAlarms();
	}
}

void systemTimer::oncePerSecondTimer(){
	if(timerOneSecTicks >= 10){
		cli();
		timerOneSecTicks = 0;
		sei();
		alarms.checkAlarms();
	}
}

ISR(TIMER1_COMPA_vect){
	i2c_timeout_ticks++;
	nvm_timeout_ticks++;
	timerOneSecTicks++;
	timerTenSecTicks++;
}
