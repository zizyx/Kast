#include "alarm.h"

struct alarm_t alarms[MAX_NUMBER_ALARMS];

alarm::alarm(DS_3231 clock)
: m_clock(clock)
{
	for (int i = 0; i < MAX_NUMBER_ALARMS; ++i)
	{
		alarms[i].executeTime = datetime_t(0, 0, 0, 0, 0, 0, 0);
		alarms[i].func = NULL;
		alarms[i].active = false;
	}
}

void doMij() {
	PINB = (1<<5);
}

void alarm::addNewAlarm(datetime_t executeTime, void (*func)(void)) {
	for (uint8_t i = 0; i < MAX_NUMBER_ALARMS; i++){
		if(!alarms[i].active){
			alarms[i].active = true;
			alarms[i].executeTime = executeTime;
			alarms[i].func = func;
			break;
		}
	}
}

void alarm::addNewAlarm(datetime_t executeTime) {
	addNewAlarm(executeTime, doMij);
}

void alarm::checkAlarms(){
	datetime_t now;

	if (m_clock.getCurrentTime(&now) == false)
		return;

// 	if (IS_TIME_INVALID(now))
// 		return;

// 	for (uint8_t i = 0; i < MAX_NUMBER_ALARMS; i++){
// 		if(alarms[i].active){
// 			if((alarms[i].executeTime <= now)){
// 				if (alarms[i].func != NULL)
// //					alarms[i].func();
// 				// if funct not null, do funt
// 				alarms[i].active = false;
// 			}
// 		}
// 	}
}
