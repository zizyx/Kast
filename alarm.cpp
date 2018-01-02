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

void doMij() {
	uint16_t crc;

	uint16_t len = 6;
	uint8_t data[6] = {6, 2, 3, 4, 5, 0};
	char str[80];

	PINB = (1<<5);

	crc = 0x00;

	for (uint16_t j = len, data_idx = 0; j > 0; j--, data_idx++) {
		crc ^= (data[data_idx] << 8);

		for (uint8_t i = 8; i; i--) {
			if (crc & 0x8000){
				crc ^= (0x1070 << 3);
			}
			crc <<= 1;
		}
		sprintf(str, "crc = %i \n", crc);
		DEBUG_STR(str);
	}

	crc >>= 8;

	sprintf(str, "crc = %u\n", (uint8_t)crc);
	DEBUG_STR(str);	
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
	datetime_t now = clock->getCurrentTime();

	for (uint8_t i = 0; i < MAX_NUMBER_ALARMS; i++){
		if(alarms[i].active){
			if((alarms[i].executeTime <= now)){
				if (alarms[i].func != NULL)
					alarms[i].func();
				// if funct not null, do funt
				alarms[i].active = false;
			}
		}
	}
}
