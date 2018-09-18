#ifndef DS_3231_H
#define DS_3231_H

#include "i2c.h"
#include "datetime.h"
#include "uart.h"

#include <inttypes.h> //includes stdint
#include <stdio.h>

#define REGISTER_AMOUNT					7
#define DS3231_ADRESS 				 	0x68

//MSB is the integer part of the temperature. 
#define DS3231_REGISTER_TEMP_MSB		0x11

//MSB is the fractional part of the temperature. 
#define DS3231_REGISTER_TEMP_LSB		0x12

#define DS3231_REGISTER_SECONDS			0x00
#define DS3231_REGISTER_MINUTES			0x01
#define DS3231_REGISTER_HOURS			0x02
#define DS3231_REGISTER_DAY				0x03
#define DS3231_REGISTER_DATE			0x04
#define DS3231_REGISTER_MONTH			0x05
#define DS3231_REGISTER_YEAR			0x06

#define DS_3231_SECONDS_MASK 			0b00001111
#define DS_3231_TEN_SECONDS_MASK 		0b01110000
#define DS_3231_MINUTES_MASK			0b00001111
#define DS_3231_TEN_MINUTES_MASK		0b01110000
#define DS_3231_HOURS_MASK				0b00001111
#define DS_3231_TEN_HOURS_MASK			0b00010000
#define DS_3231_TWENTY_HOURS_MASK		0b00100000
#define DS_3231_DAY_MASK				0b00000111
#define DS_3231_DATE_MASK				0b00001111
#define DS_3231_TEN_DATE_MASK			0b00110000
#define DS_3231_MONTH_MASK				0b00001111
#define DS_3231_TEN_MONTH_MASK			0b00010000
#define DS_3231_CENTURY_MASK			0b10000000
#define DS_3231_YEAR_MASK				0b00001111
#define DS_3231_TEN_YEAR_MASK			0b11110000

#define DS_3231_SECONDS_POSITION 		(0)
#define DS_3231_TEN_SECONDS_POSITION 	(4)
#define DS_3231_MINUTES_POSITION		(0)
#define DS_3231_TEN_MINUTES_POSITION	(4)
#define DS_3231_HOURS_POSITION			(0)
#define DS_3231_TEN_HOURS_POSITION		(4)
#define DS_3231_TWENTY_HOURS_POSITION	(5)
#define DS_3231_DAY_POSITION			(0)
#define DS_3231_DATE_POSITION			(0)
#define DS_3231_TEN_DATE_POSITION		(4)
#define DS_3231_MONTH_POSITION			(0)
#define DS_3231_TEN_MONTH_POSITION		(4)
#define DS_3231_CENTURY_POSITION		(7)
#define DS_3231_YEAR_POSITION			(0)
#define DS_3231_TEN_YEAR_POSITION		(4)

#define DS_3231_GET_VAL(reg, mask, position) ( (reg & mask) >> position  )

#define DS_3231_SET_SECONDS(sec) ( ( ((sec % 10) << DS_3231_SECONDS_POSITION) & DS_3231_SECONDS_MASK) | \
											( ( ( (sec - (sec % 10)) / 10)  << DS_3231_TEN_SECONDS_POSITION) & DS_3231_TEN_SECONDS_MASK) )

#define DS_3231_SET_MINUTES(min) ( ( ((min % 10) << DS_3231_MINUTES_POSITION) & DS_3231_MINUTES_MASK) | \
											( ( ( (min - (min % 10)) / 10)  << DS_3231_TEN_MINUTES_POSITION) & DS_3231_TEN_MINUTES_MASK) )

#define DS_3231_SET_HOURS(hours) ( ( ((hours % 10) << DS_3231_HOURS_POSITION) & DS_3231_HOURS_MASK) | \
											( ( ( (hours - (hours % 10)) / 10)  << DS_3231_TEN_HOURS_POSITION) & DS_3231_TEN_HOURS_MASK) | \
											( ( ( (hours >= 20 ? 1 : 0 )  << DS_3231_TWENTY_HOURS_POSITION) & DS_3231_TWENTY_HOURS_MASK) ) )

//Boven werkt, onder niet
#define DS_3231_SET_DAY(day) ( (day << DS_3231_DAY_POSITION) & DS_3231_DAY_MASK)

#define DS_3231_SET_DATE(date) ( ( ((date % 10) << DS_3231_DATE_POSITION) & DS_3231_DATE_MASK) | \
											( ( ( (date - (date % 10)) / 10)  << DS_3231_TEN_DATE_POSITION) & DS_3231_TEN_DATE_MASK) )

#define DS_3231_SET_MONTH(month, year) ( ( ((month % 10) << DS_3231_MONTH_POSITION) & DS_3231_MONTH_MASK) | \
											( ( ( (month - (month % 10)) / 10)  << DS_3231_TEN_MONTH_POSITION) & DS_3231_TEN_MONTH_MASK) | \
                                            ( ( ( ((year - 2000 - ((year - 2000)) % 100)) / 100)  << DS_3231_CENTURY_POSITION) & DS_3231_CENTURY_MASK) )

#define DS_3231_SET_YEAR(year) ( ( ((year % 10) << DS_3231_YEAR_POSITION) & DS_3231_YEAR_MASK) | \
											( ( ( (year % 100 - (year % 10)) / 10)  << DS_3231_TEN_YEAR_POSITION) & DS_3231_TEN_YEAR_MASK) )
class i2c;
class uart;

struct DS3231_data {
	// Register values
	uint8_t reg_temp;
	uint8_t reg_seconds;
	uint8_t reg_minutes;
	uint8_t reg_hours;
	uint8_t reg_day;
	uint8_t reg_date;
	uint8_t reg_month;
	uint16_t reg_year;

	datetime_t datetime;
};

class DS_3231 {
	public:
		DS_3231(void);
		uint16_t ReadTemperature();
		void getTimeVarsAsString(char *string);
		bool getCurrentTime(datetime_t *time);
		void testing(char* string);
		bool setTime(datetime_t datetime);

	private:
		i2c *I2C;
		uart *debug;

		char* dayToString(uint8_t day);
		bool ReadClockRegisters();
		bool ReadTempRegisters();
		void calcClock();
};


#endif