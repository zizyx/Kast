#ifndef PLANTS_H
#define PLANTS_H

#include <inttypes.h>

#define KUSH 0
#define NORTHERN_LIGHTS 1

struct plants {
	uint8_t plant_id;
	uint8_t max_temp;
	uint8_t min_temp;
	uint8_t min_humidity;
	uint8_t max_humidity;
	uint16_t lamp_one_time; //minutes
	uint16_t lamp_off_time; //minutes
	uint8_t grow_time; //days
	uint8_t watering_cooldown; //Hours, Some plants need the ground to become dry after watering.
};

/*
struct plants plant_list[] = {
	{
		KUSH, //plant_id
		50, //max_temp
		5, //min_temp
		10, //min_humidity
		100, //max_humidity
		5, //lamp_one_time
		5, //lamp_off_time
		90, //grow_time
		10, //watering_cooldown
	},	
	{
		NORTHERN_LIGHTS, //plant_id
		50, //max_temp
		5, //min_temp
		10, //min_humidity
		100, //max_humidity
		5, //lamp_one_time
		5, //lamp_off_time
		90, //grow_time
		10, //watering_cooldown
	},
};
*/
#endif