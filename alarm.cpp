#include "alarm.h"

struct alarm_t alarms[MAX_NUMBER_ALARMS] =  {
	{
		datetime_t(0, 0, 0, 0, 0, 0, 0),
		NULL,
		false
	}	
};

alarm::alarm()
: clock(DS_3231::getInstance())
{
}

void alarm::addNewAlarm(datetime_t executeTime){
	for (uint8_t i = 0; i < MAX_NUMBER_ALARMS; i++){
		if(!alarms[i].active){
			alarms[i].active = true;
			alarms[i].executeTime = executeTime;
			alarms[i].functionName = NULL;
			break;
		}
	}
}

void alarm::checkAlarms(){
	datetime_t now = clock->getCurrentTime();

	for (uint8_t i = 0; i < MAX_NUMBER_ALARMS; i++){
		if(alarms[i].active){
			if((alarms[i].executeTime <= now)){
				PINB = (1<<5);
				// if funct not null, do funt
				alarms[i].active = false;
			}
		}
	}
}
