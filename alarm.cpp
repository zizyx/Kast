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
	uint16_t len = 6;
	uint8_t data[6] = {0, 2, 3, 4, 5, 255};
	uint8_t crc, crc8;
	char str[80];

	PINB = (1<<5);

	crc = 0x00;
	crc8 = 0x00;

	for (uint16_t i = 0; i < len; i++) {
		crc ^= data[i];

		for (uint8_t j = 0; j < 8; j++) {
			if (crc & 0x01) {
                crc = (crc >> 1) ^ 0xA001;
			} else {
				crc = (crc >> 1);
			}
			crc |= (crc << i);
		}

		crc8 ^= crc;
	}
	sprintf(str, "crc = %i\n", crc8);
	DEBUG_STR(str);

	// return crc;	
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
