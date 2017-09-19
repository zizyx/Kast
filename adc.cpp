#include "adc.h"

//Singleton
adc adc::instance = adc();

adc::adc(void) {
	Init();
}

void adc::Init(void) {
	//16 	/128  125khz
	setAdcPrescaler(DIVISION_FACTOR_128X);
	setVoltageReference(AVCC);
	// setChannel(CHANNEL_TEMPERATURE);
	// enableInterrupt();
	setAdjustResultDirection(RIGHT_ADJUST_RESULT); //Difficult, don't touch
	enableAdc();
}

adc *adc::getInstance() {
	return &instance;
}

void adc::setVoltageReference(uint8_t setting){
	ADMUX &= ~(0x03 << REFS0);
	ADMUX |= (setting << REFS0);
}


void adc::setChannel(uint8_t channel) {
	ADMUX &= 0xF0; //RESET
	ADMUX |= (channel & 0x0F);
}

void adc::enableAdc(void) {
	ADCSRA |= (1 << ADEN);
}

void adc::disableAdc(void) {
	ADCSRA &= ~(1 << ADEN);
}

void adc::startFirstConversion(void) {
	ADCSRA |= (1 << ADSC);
}

void adc::enableInterrupt(void) {
	ADCSRA |= (1 << ADIE);
}

void adc::disableInterrupt(void) {
	ADCSRA &= ~(1 << ADIE);
}

void adc::setAdcPrescaler(uint8_t division_factor) {
	ADCSRA &= 0xF8;
	ADCSRA |= (0x07 & division_factor);
}

//Always 10 bit precision
uint16_t adc::readAdc() {
	uint8_t adlar_setting = (ADMUX & (1 << ADLAR));
	uint16_t result = 0x00;

	if(adlar_setting) { //Left adjust
		result = (readAdcLow() << 6);
		result |= (readAdcHigh() << 2 );
	} else { //Right adjust
		result = readAdcLow();
		result |= (readAdcHigh() << 8 );
	}

	return result;
}

uint8_t adc::readAdcLow() {
	return ADCL;
}

uint8_t adc::readAdcHigh() {
	//Read low first.
	return ADCH;
}

void adc::setAdjustResultDirection(uint8_t mode) {
	ADMUX &= ~(1 << ADLAR);
	ADMUX |= (mode << ADLAR);
}
