#include "nvm.h"
#include "plants.h"

#define	PLANT_SIZE						sizeof(struct plant_t)
#define	PLANT_OFFSET 					(EEPROM_VALID_OFFSET + 1)
#define	PLANT_CRC_OFFSET				(PLANT_OFFSET + PLANT_SIZE + 1)
