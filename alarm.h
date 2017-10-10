#ifndef ALARM_H
#define ALARM_H

#include <avr/io.h>
#include <inttypes.h>
#include <string.h>
#include "datetime.h"
#include "uart.h"
#include "DS_3231.h"

#define MAX_NUMBER_ALARMS 			10

struct alarm_t{
	datetime_t executeTime;
	void* functionName;
	bool active;
};

class alarm {
	public:
		alarm();
		void checkAlarms();
		void addNewAlarm(datetime_t executeTime);

	private:
		uart *uartHandler;
		DS_3231 *clock;
		void init();
};

#endif
