#ifndef DATETIME_H
#define DATETIME_H

#include <inttypes.h>
#include <stdio.h>

#define INVALID_DATE_TIME           datetime_t(0, 0, 0, 0, 0, 0, 2000)
#define IS_TIME_INVALID(x)          ((x == INVALID_DATE_TIME) ? true : false)
#define IS_TIME_VALID(x)            (!IS_TIME_INVALID(x))

enum Day { monday = 1, tuesday, wednesday, thursday, friday, saturday, sunday};

struct datetime_t {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint16_t year;

	datetime_t() {}
	datetime_t(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t day, uint8_t date, 
		uint8_t month, uint16_t year) : seconds(seconds), 
		minutes(minutes), hours(hours), day(day), date(date), month(month), year(year) {}

	datetime_t& operator=(const datetime_t& a)
	{
		seconds = a.seconds;
		minutes = a.minutes;
		hours = a.hours;
		day = a.day;
		date = a.date;
		month = a.month;
		year = a.year;

		return *this;
	}

	datetime_t operator-(const datetime_t& a) const
	{
		return datetime_t(seconds - a.seconds, minutes - a.minutes, hours - a.hours, day - a.day, date - a.date, month - a.month, year - a.year);
	}

	datetime_t operator+(const datetime_t& a) const
	{
		return datetime_t(seconds + a.seconds, minutes + a.minutes, hours + a.hours, day + a.day, date + a.date, month + a.month, year + a.year);
	}

   bool operator>(const datetime_t& a) const
    {
        if(year > a.year){
            return true;
        }else if(year < a.year){
            return false;
        }else if(month > a.month){
            return true;
        }else if(month < a.month){
            return false;
        }else if(date > a.date){
            return true;
        }else if(date < a.date){
            return false;
        }else if(hours > a.hours){
            return true;
        }else if(hours < a.hours){
            return false;
        }else if(minutes > a.minutes){
            return true;
        }else if(minutes < a.minutes){
            return false;
        }else if(seconds > a.seconds){
            return true;
        }else if(seconds < a.seconds){
            return false;
        }
    }
 
    bool operator>=(const datetime_t& a) const
    {
        if(seconds == a.seconds && minutes == a.minutes && hours == a.hours && day == a.day && date == a.date && month == a.month && year == a.year){
            return true;
        }else if(year > a.year){
            return true;
        }else if(year < a.year){
            return false;
        }else if(month > a.month){
            return true;
        }else if(month < a.month){
            return false;
        }else if(date > a.date){
            return true;
        }else if(date < a.date){
            return false;
        }else if(hours > a.hours){
            return true;
        }else if(hours < a.hours){
            return false;
        }else if(minutes > a.minutes){
            return true;
        }else if(minutes < a.minutes){
            return false;
        }else if(seconds > a.seconds){
            return true;
        }else if(seconds < a.seconds){
            return false;
        }
        return false;
    }
 
     bool operator<(const datetime_t& a) const
    {
        if(year < a.year){
            return true;
        }else if(year > a.year){
            return false;
        }else if(month < a.month){
            return true;
        }else if(month > a.month){
            return false;
        }else if(date < a.date){
            return true;
        }else if(date > a.date){
            return false;
        }else if(hours < a.hours){
            return true;
        }else if(hours > a.hours){
            return false;
        }else if(minutes < a.minutes){
            return true;
        }else if(minutes > a.minutes){
            return false;   
        }else if(seconds < a.seconds){
            return true;
        }else if(seconds > a.seconds){
            return false;
        }return false;
    }
 
    bool operator<=(const datetime_t& a) const
    {
        if(seconds == a.seconds && minutes == a.minutes && hours == a.hours && day == a.day && date == a.date && month == a.month && year == a.year){
            return true;
        }else if(year < a.year){
            return true;
        }else if(year > a.year){
            return false;
        }else if(month < a.month){
            return true;
        }else if(month > a.month){
            return false;
        }else if(date < a.date){
            return true;
        }else if(date > a.date){
            return false;
        }else if(hours < a.hours){
            return true;
        }else if(hours > a.hours){
            return false;
        }else if(minutes < a.minutes){
            return true;
        }else if(minutes > a.minutes){
            return false;   
        }else if(seconds < a.seconds){
            return true;
        }else if(seconds > a.seconds){
            return false;
        }return false;
    }

	bool operator==(const datetime_t& a) const
	{
		return (seconds == a.seconds && minutes == a.minutes && hours == a.hours && day == a.day && date == a.date && month == a.month && year == a.year);
	}

	char *toString() {
		sprintf(datetime_as_string, "%02u:%02u:%02u %s %02u-%02u-%02u",
			hours,
			minutes,
			seconds,
			dayToString(day),
			date,
			month,
			year
		);

		return datetime_as_string;
	}

	private:
		char datetime_as_string[30];

		const char *dayToString(uint8_t day){
			switch (day) {
				case monday: 		return (const char *)"monday";
				case tuesday: 		return (const char *)"tuesday";
				case wednesday: 	return (const char *)"wednesday";
				case thursday: 		return (const char *)"thursday";
				case friday: 		return (const char *)"friday";
				case saturday: 		return (const char *)"saturday";
				case sunday: 		return (const char *)"sunday";
				default:			return (const char *)"420-day";
			}
		}


};

#endif