#ifndef PLANTS_H
#define PLANTS_H

#include <inttypes.h>
#include "datetime.h"

#define INVALID_PLANT_ID	0

struct plant_t {
	uint8_t id;
	uint8_t max_temp;
	uint8_t min_temp;
	uint8_t min_humidity;
	uint8_t max_humidity;
	uint16_t lamp_one_time; //minutes
	uint16_t lamp_off_time; //minutes
	datetime_t grow_duration; //days
	uint8_t watering_cooldown; //Hours, Some plants need the ground to become dry after watering.
	datetime_t grow_start_date;
	bool growing_started;
};

#endif