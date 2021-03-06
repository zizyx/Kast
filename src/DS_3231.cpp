/////////////////////////////////////////////////////////////////////////////////
// DS3231 - RealTimeClock
/////////////////////////////////////////////////////////////////////////////////
#include "DS_3231.h"
#include <util/delay.h>

static DS_3231 *instance = NULL;

static DS3231_data clk_data;

DS_3231::DS_3231(){
#ifdef DS3231_RAW_REGISTERS
	debug = uart::getInstance();
#endif
}

DS_3231 *DS_3231::getInstance(){
	if (!instance) {
		instance = &DS_3231();
	}

	return instance;
}

void DS_3231::ReadClockRegisters(){
	uint8_t buffer[7];
	I2C->ReadRegisterFlow(DS3231_ADRESS, DS3231_REGISTER_SECONDS, REGISTER_AMOUNT, buffer);

	clk_data.reg_seconds 			= DS_3231_GET_VAL( buffer[0], 	DS_3231_SECONDS_MASK, 		DS_3231_SECONDS_POSITION)
								+ (10 * DS_3231_GET_VAL( buffer[0], 	DS_3231_TEN_SECONDS_MASK, 	DS_3231_TEN_SECONDS_POSITION ));

	clk_data.reg_minutes 			= DS_3231_GET_VAL( buffer[1], 		DS_3231_MINUTES_MASK, 		DS_3231_MINUTES_POSITION )
								+ (10 * DS_3231_GET_VAL( buffer[1], 	DS_3231_TEN_MINUTES_MASK, 	DS_3231_TEN_MINUTES_POSITION ));

	clk_data.reg_hours 				= DS_3231_GET_VAL( buffer[2], 			DS_3231_HOURS_MASK, 		DS_3231_HOURS_POSITION )
								+ (10 * DS_3231_GET_VAL( buffer[2], 	DS_3231_TEN_HOURS_MASK, 	DS_3231_TEN_HOURS_POSITION ))
								+ (20 *  DS_3231_GET_VAL( buffer[2], 	DS_3231_TWENTY_HOURS_MASK, 	DS_3231_TWENTY_HOURS_POSITION ));

	clk_data.reg_day 				= DS_3231_GET_VAL( buffer[3], 			DS_3231_DAY_MASK, 			DS_3231_DAY_POSITION );

	clk_data.reg_date 				= DS_3231_GET_VAL( buffer[4],			DS_3231_DATE_MASK,			DS_3231_DATE_POSITION )
								+ (10 * DS_3231_GET_VAL( buffer[4],		DS_3231_TEN_DATE_MASK, 		DS_3231_TEN_DATE_POSITION ));

	clk_data.reg_month 				= DS_3231_GET_VAL( buffer[5], 			DS_3231_MONTH_MASK, 		DS_3231_MONTH_POSITION )
								+ (10 * DS_3231_GET_VAL( buffer[5], 	DS_3231_TEN_MONTH_MASK,		DS_3231_TEN_MONTH_POSITION ));

	clk_data.reg_year				= DS_3231_GET_VAL( buffer[6], 			DS_3231_YEAR_MASK, 			DS_3231_YEAR_POSITION )
								+ (10 * DS_3231_GET_VAL( buffer[6],		DS_3231_TEN_YEAR_MASK, 		DS_3231_TEN_YEAR_POSITION )) 
								+ (100 * DS_3231_GET_VAL( buffer[5], 	DS_3231_CENTURY_MASK,		DS_3231_CENTURY_POSITION ))
								+ 2000;

	clk_data.datetime = datetime_t(
		clk_data.reg_seconds,
		clk_data.reg_minutes,
		clk_data.reg_hours,
		clk_data.reg_day,
		clk_data.reg_date,
		clk_data.reg_month,
		clk_data.reg_year);
}

uint16_t DS_3231::ReadTemperature(){
	ReadTempRegisters();
	return clk_data.reg_temp;
}

void DS_3231::ReadTempRegisters(){
	clk_data.reg_temp = I2C->ReadRegisteru8(DS3231_ADRESS, DS3231_REGISTER_TEMP_MSB);
}

datetime_t DS_3231::getCurrentTime() {
	ReadClockRegisters();

	return clk_data.datetime;
}

void DS_3231::getTimeVarsAsString(char *string) {
	getCurrentTime();

	sprintf(string, "%s\n",
		clk_data.datetime.toString());
}

void DS_3231::setTime(datetime_t datetime) {
	uint8_t sec_reg = DS_3231_SET_SECONDS(datetime.seconds);
	uint8_t min_reg = DS_3231_SET_MINUTES(datetime.minutes);
	uint8_t hr_reg = DS_3231_SET_HOURS(datetime.hours);
	uint8_t dy_reg = DS_3231_SET_DAY(datetime.day);
	uint8_t dt_reg = DS_3231_SET_DATE(datetime.date);
	uint8_t mn_reg = DS_3231_SET_MONTH(datetime.month, datetime.year);
	uint8_t yr_reg = DS_3231_SET_YEAR(datetime.year);

	I2C->WriteData(DS3231_ADRESS, DS3231_REGISTER_SECONDS, sec_reg, true);
	I2C->WriteData(DS3231_ADRESS, DS3231_REGISTER_MINUTES, min_reg, true);
	I2C->WriteData(DS3231_ADRESS, DS3231_REGISTER_HOURS, hr_reg, true);
	I2C->WriteData(DS3231_ADRESS, DS3231_REGISTER_DAY, dy_reg, true);
	I2C->WriteData(DS3231_ADRESS, DS3231_REGISTER_DATE, dt_reg, true);
	I2C->WriteData(DS3231_ADRESS, DS3231_REGISTER_MONTH, mn_reg, true);
	I2C->WriteData(DS3231_ADRESS, DS3231_REGISTER_YEAR, yr_reg, true);
}

char* DS_3231::dayToString(uint8_t day){
	switch (day) {
	    case monday: 		return (char *)"monday";
	    case tuesday: 		return (char *)"tuesday";
	    case wednesday: 	return (char *)"wednesday";
	    case thursday: 		return (char *)"thursday";
	    case friday: 		return (char *)"friday";
	    case saturday: 		return (char *)"saturday";
	    case sunday: 		return (char *)"sunday";
	    default:			return (char *)"420-day";
	}
}