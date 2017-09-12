#include "plants.h"

struct plant plants[NR_OF_PLANTS] =  {
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
	}
};
