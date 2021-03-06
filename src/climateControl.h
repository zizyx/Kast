#ifndef CLIMATE_CONTROL_H
#define CLIMATE_CONTROL_H

#include <inttypes.h>
#include "plants.h"
#include "i2c.h"
#include "uartCallback.h"
#include "pdm.h" 
#include "DS_3231.h"
#include "BMP_280.h"
#include "datetime.h"
#include <math.h>

#define FAN_ON 1
#define FAN_OFF 0
#define LAMP_ON 1
#define LAMP_OFF 0
#define WATER_PUMP_ON 1
#define WATER_PUMP_OFF 0
#define WATER_PUMP_TIMEOUT 100 //?
#define OUTSIDE_BAROMETER_ID '0'
#define INSIDE_BAROMETER_ID '1'
#define BMP280_ADDRESS_INSIDE 			 	 ADDRESS_LOW
#define BMP280_ADDRESS_OUTSIDE 				 ADDRESS_HIGH

#define LAMP_ONE 0
#define LAMP_TWO 1 //Pin nr
#define LAMPS 2

#define UPPER_PRESSURE_TOLERANCE 10
#define LOWER_PRESSURE_TOLERANCE 5
#define MAX_PRESSURE_DIFFERENCE (UPPER_PRESSURE_TOLERANCE * 2)

#define MAX_FAN_PWM 0
#define MIN_FAN_PWM 255
#define PWM_STEP 1

//Uart messages
#define CMD_FAN_ON "fan_on"
#define CMD_FAN_ON_LEN 6
#define CMD_FAN_OFF "fan_off"
#define CMD_FAN_OFF_LEN 7

#define CMD_FAN_LVL "fan_lvl_"
#define CMD_FAN_LVL_ARG_LEN 3
#define CMD_FAN_LVL_LEN (8 + CMD_FAN_LVL_ARG_LEN)

#define CMD_GET_TEMP "get_temp_"
#define CMD_GET_TEMP_ARG_LEN 1
#define CMD_GET_TEMP_LEN (9 + CMD_GET_TEMP_ARG_LEN)

#define CMD_GET_CLOCK "get_clock"
#define CMD_GET_CLOCK_LEN 9

#define CMD_SET_CLOCK "set_clock_"
#define CMD_SET_CLOCK_ARG_LEN 21
#define	CMD_SET_CLOCK_LEN (10 + CMD_SET_CLOCK_ARG_LEN)
#define CLOCK_SECOND_OFFSET (CMD_SET_CLOCK_LEN - CMD_SET_CLOCK_ARG_LEN + 0)
#define CLOCK_MINUTE_OFFSET (CMD_SET_CLOCK_LEN - CMD_SET_CLOCK_ARG_LEN + 3)
#define CLOCK_HOUR_OFFSET (CMD_SET_CLOCK_LEN - CMD_SET_CLOCK_ARG_LEN + 6)
#define CLOCK_DAY_OFFSET (CMD_SET_CLOCK_LEN - CMD_SET_CLOCK_ARG_LEN + 9)
#define CLOCK_DATE_OFFSET (CMD_SET_CLOCK_LEN - CMD_SET_CLOCK_ARG_LEN + 11)
#define CLOCK_MONTH_OFFSET (CMD_SET_CLOCK_LEN - CMD_SET_CLOCK_ARG_LEN + 14)
#define CLOCK_YEAR_OFFSET (CMD_SET_CLOCK_LEN - CMD_SET_CLOCK_ARG_LEN + 17)

#define CLOCK_SECOND_OFFSET_LEN  2
#define CLOCK_MINUTE_OFFSET_LEN  2
#define CLOCK_HOUR_OFFSET_LEN  2
#define CLOCK_DAY_OFFSET_LEN  1
#define CLOCK_DATE_OFFSET_LEN  2
#define CLOCK_MONTH_OFFSET_LEN  2
#define CLOCK_YEAR_OFFSET_LEN  4


struct climateVars {
	uint32_t 		inside_temp;
	uint32_t 		outside_temp;

	int64_t 		inside_pressure;
	int64_t 		outside_pressure;

	bool 			fan_status;
	uint8_t 		fan_pwm_level;

	bool 			lamp_status[LAMPS];
	uint16_t 		lamp_time[LAMPS];

	bool 			water_pump_status;
	bool 			water_pump_time;

	uint16_t 		humidity;
	struct plants	selected_plant;
};


class climateControl : public uartCallback{
	public:
		static climateControl *getInstance();
		static void handleCmd(char *cmd, uint8_t cmdLength);
		void checkClimate();
	    // The callback function that Caller will call.
//	    int cbiCallbackFunction(int i);
		int cbiCallbackFunction(char*, int length);/* {
		//	TransmitString("10,");
	   		return 2 * i; 
		}*/

	private:
		static climateControl *instance;
		i2c *I2C;
		pdm *Pdm;
		DS_3231 *clock;
		BMP_280 baro_inside;
		BMP_280 baro_outside;
		uart *uartHandler;

		climateControl(void);
		struct climateVars vars;
		bool isClimateSafetyActive();
		void updateClimateVars();
		void setLampState(uint8_t lamp_id, uint8_t status);
		void setFanState(uint8_t status, uint8_t pwm);
		void setFanState(uint8_t status);
		void setWaterPumpState(uint8_t status);
		int64_t getPressure(uint8_t barometer_id);
		uint8_t getTemperature(char barometer_id);
		uint16_t getHumidity();
		void calculateClimateVars();
		void calculateFanPwmVars();
		void calculateLampVars(uint8_t lamp_id);
		void calculateWaterPumpVars();
		void setPlantToGrow(uint16_t plant_id);
		void updateHardware();
		void setFanHardware();
		void setLampHardware(uint8_t lamp_id);
		void setWaterPumpHardware();
		static uint16_t stringToUint16(char* str, uint8_t str_len);

};

#endif












