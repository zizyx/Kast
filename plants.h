#ifndef PLANTS_H
#define PLANTS_H

#include <inttypes.h>

#define NO_PLANT 0
#define KUSH 1
#define NORTHERN_LIGHTS 2


struct plant{
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

#define NR_OF_PLANTS 2

extern struct plant plants[NR_OF_PLANTS];

#endif