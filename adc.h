#ifndef ADC_H
#define ADC_H

#include <inttypes.h>
#include <avr/io.h>
#include <stdlib.h>

#define AREF 		0x00
#define AVCC 		0x01
#define INTERNAL 	0x03

#define CHANNEL_ZERO 		0x00
#define CHANNEL_ONE 		0x01
#define CHANNEL_TWO			0x02
#define CHANNEL_THREE		0x03
#define CHANNEL_FOUR		0x04
#define CHANNEL_FIVE		0x05
#define CHANNEL_SIX			0x06
#define CHANNEL_SEVEN		0x07
#define CHANNEL_TEMPERATURE	0x08
#define CHANNEL_ONE_V_ONE	0x0E
#define CHANNEL_GROUND		0x0F

#define DIVISION_FACTOR_2X 		0x01
#define DIVISION_FACTOR_4X 		0x02
#define DIVISION_FACTOR_8X 		0x03
#define DIVISION_FACTOR_16X 	0x04
#define DIVISION_FACTOR_32X 	0x05
#define DIVISION_FACTOR_64X 	0x06
#define DIVISION_FACTOR_128X 	0x07

#define LEFT_ADJUST_RESULT 		0x01
#define RIGHT_ADJUST_RESULT 	0x00

class adc {
	public:
		static adc *getInstance();
		void setVoltageReference(uint8_t setting);
		void setChannel(uint8_t channel);
		void enableAdc(void);
		void disableAdc(void);
		void startFirstConversion(void);
		void enableInterrupt(void);
		void disableInterrupt(void);
		void setAdcPrescaler(uint8_t division_factor);
		void setAdjustResultDirection(uint8_t mode); 
		uint16_t readAdc();

	private:
		static adc instance;
		adc(void);
		void Init(void);
		uint8_t readAdcLow();
		uint8_t readAdcHigh();
};

#endif
