#include "climateControl.h"
#include <math.h>

climateControl *climateControl::instance = NULL;

climateControl *climateControl::getInstance(){
	if (instance == NULL) {
		instance = &climateControl();
	} 
	return instance; 	
}

static uint16_t stringToUint16(char* str, uint8_t str_len) {
	double val = 0;
	for (int i = 0, j = str_len - 1; i < str_len; i++, j--) {
		val += (str[i] - '0') * pow(10, j);
		// uart::getInstance()->Transmit('\n'); //Echo what is sent to the device	
		// uart::getInstance()->Transmit(str[i] - '0'); //Echo what is sent to the device	
		// uart::getInstance()->TransmitString(" --- "); //Echo what is sent to the device	
		// uart::getInstance()->Transmit(val); //Echo what is sent to the device	
	}
	return round(val);
}

static uint8_t stringToUint8(char* str, uint8_t str_len) {
	return (uint8_t)stringToUint16(str, str_len);
}


climateControl::climateControl() 
: baro_inside(BMP280_ADDRESS_INSIDE) , baro_outside(BMP280_ADDRESS_OUTSIDE)
 {
	setPlantToGrow(KUSH);
	// uartHandler = uart::getInstance();
	clock = DS_3231::getInstance();
	// baro_inside = BMP_280::BMP_280(BMP280_ADRESS_INSIDE);
	// baro_outside = BMP_280::BMP_280(BMP280_ADRESS_OUTSIDE);
}

int climateControl::cbiCallbackFunction(char* string, int length) {
	// uartHandler->TransmitString(string);
	handleCmd(string, length);
    return 1; //For checking if the handler is valid.
}


void climateControl::checkClimate() {
	//Update climate vars from sensors.
	updateClimateVars();

	if (isClimateSafetyActive()) {
		//CalculateClimateVars with vars from sensors.
		calculateClimateVars();
	}

	//Align hardware settings with vars settings.
	updateHardware();
}

bool climateControl::isClimateSafetyActive() {
	bool active = false;
	if ( vars.inside_temp >= vars.selected_plant.max_temp ) {
		active = true;
		vars.lamp_status[LAMP_ONE] = LAMP_OFF;
		vars.lamp_status[LAMP_TWO] = LAMP_OFF;	
		setLampState(LAMP_ONE, LAMP_OFF);
		setLampState(LAMP_TWO, LAMP_OFF);
		setFanState(FAN_ON, MAX_FAN_PWM); //Cool that plant. max
	}

	if ( vars.humidity >= vars.selected_plant.max_humidity ) {
		active = true;
		setWaterPumpState(WATER_PUMP_OFF);
	}

	if ( vars.inside_pressure >= ( vars.inside_pressure + (UPPER_PRESSURE_TOLERANCE * 2) ) ) {
		active = true;
		setFanState(FAN_OFF, 0);
	}

	return active;
}


//UPDATE
void climateControl::updateClimateVars() {
	vars.outside_pressure = getPressure(BMP280_ADDRESS_OUTSIDE);
	vars.outside_temp = getTemperature(BMP280_ADDRESS_OUTSIDE);

	vars.inside_pressure = getPressure(BMP280_ADDRESS_INSIDE);
	vars.inside_temp = getTemperature(BMP280_ADDRESS_INSIDE);

	vars.humidity = getHumidity();
}

//Vars setters
void climateControl::setLampState(uint8_t lamp_id, uint8_t status) {
		vars.lamp_status[lamp_id] = status;
		vars.lamp_time[lamp_id] = 0; //Update timer value
}

void climateControl::setFanState(uint8_t status) {
	setFanState(status, vars.fan_pwm_level);
}

void climateControl::setFanState(uint8_t status, uint8_t pwm) {
	if (status == FAN_ON) {
		vars.fan_status = FAN_ON;
	} else if (status == FAN_OFF) {
		vars.fan_status = FAN_OFF;
	}

	vars.fan_pwm_level = pwm;
	uart::getInstance()->TransmitString("setFanState");
	uart::getInstance()->Transmit(vars.fan_pwm_level);
}

void climateControl::setWaterPumpState(uint8_t status) {
	vars.water_pump_status = status;
	vars.water_pump_time = 0; //Update timer value
}

//Vars getters
int64_t climateControl::getPressure(uint8_t barometer_id) { 
	// baro_inside.ReadPressure();
	// baro_inside.ReadTemperatureRound();
	//I2C
	return 0;
} 

//Used for both pressure sensors
uint8_t climateControl::getTemperature(char barometer_id) {
	if (barometer_id == OUTSIDE_BAROMETER_ID)
		return baro_outside.ReadTemperatureRound();
	else /* OUTSIDE */
		return baro_inside.ReadTemperatureRound();
} 

uint16_t climateControl::getHumidity() { 
	//ADC
	return 0;
}


//Calls functions which calculate and update climate vaiables
void climateControl::calculateClimateVars() {
	calculateFanPwmVars();
	calculateLampVars(LAMP_ONE);
	calculateLampVars(LAMP_TWO);
	calculateWaterPumpVars();
}

void climateControl::calculateFanPwmVars() {
	if ( vars.outside_pressure >= (vars.inside_pressure + UPPER_PRESSURE_TOLERANCE ) ){
		//Decrease pressure as it is way higher than inside.
		vars.fan_pwm_level -= PWM_STEP;
	} else if ( vars.outside_pressure <= (vars.inside_pressure + LOWER_PRESSURE_TOLERANCE ) ) {
		//Outside pressure is lower than inside, increase fan now.
		vars.fan_pwm_level += PWM_STEP;
	} else {
		//Pressure is within tolerances keep pwm settings.
	}
}

void climateControl::calculateLampVars(uint8_t lamp_id) {
	vars.lamp_status[LAMP_ONE] = LAMP_ON; //or off
	vars.lamp_status[LAMP_TWO] = LAMP_ON; //or off
}

void climateControl::calculateWaterPumpVars() {
	vars.water_pump_status = WATER_PUMP_ON;
}

void climateControl::setPlantToGrow(uint16_t plant_id) {
//	vars.selected_plant = plant_list[plant_id];
}

//HARDWARE setters
void climateControl::updateHardware() {
	setFanHardware();
	setLampHardware(LAMP_ONE);
	setLampHardware(LAMP_TWO);
	setWaterPumpHardware();
}

void climateControl::setFanHardware() {
	uartHandler->TransmitString("setFanHardware ");
	uartHandler->Transmit(vars.fan_pwm_level);
	if (vars.fan_status == FAN_ON){
		Pdm->setPdm(vars.fan_pwm_level);
	}else if (vars.fan_status == FAN_OFF){
		Pdm->setPdm(MIN_FAN_PWM);
	}
	//Zet pwm regs
}

void climateControl::setLampHardware(uint8_t lamp_id) {
	// If lamp status changed update time. if not keep time.
} 

void climateControl::setWaterPumpHardware() {
	// If waterpump status changed update time. if not keep time.
}

void climateControl::handleCmd(char *cmd, uint8_t cmdLength) {
	char string[81] = {"e\n\0"};
	uart::getInstance()->TransmitString(cmd);

	// reading incoming string
	for(uint8_t i = 0; i < cmdLength; i++){
		uart::getInstance()->Transmit(cmd[i]); //Echo what is sent to the device	
	}

	// CMD fan_on;
	////////////////////////////////////////////////////////////////////////////////////
	if (uart::getInstance()->isEqual(cmd, (char *)CMD_FAN_ON, CMD_FAN_ON_LEN, cmdLength)){
		uart::getInstance()->TransmitString("\nCMD_FAN_ON ");
	 	getInstance()->setFanState(FAN_ON); //Cool that plant. max	
	} 
	// CMD fan_off;
	////////////////////////////////////////////////////////////////////////////////////
	else if (uart::getInstance()->isEqual(cmd, (char *)CMD_FAN_OFF, CMD_FAN_OFF_LEN, cmdLength)){
		uart::getInstance()->TransmitString("\nCMD_FAN_OFF ");
	 	getInstance()->setFanState(FAN_OFF); //Cool that plant. max	
	} 
	// CMD Fan_lvl_xxx;
	////////////////////////////////////////////////////////////////////////////////////
	else if (uart::getInstance()->isEqual(cmd, (char *)CMD_FAN_LVL, cmdLength - CMD_FAN_LVL_ARG_LEN, 
			CMD_FAN_LVL_LEN, cmdLength)) {

		uint8_t fan_pwm_level = 0;
		fan_pwm_level = stringToUint16(cmd + (CMD_FAN_LVL_LEN - CMD_FAN_LVL_ARG_LEN), CMD_FAN_LVL_ARG_LEN);

		uart::getInstance()->TransmitString("\nCMD_FAN_LVL_ ");
		uart::getInstance()->Transmit(fan_pwm_level);
	 	getInstance()->setFanState(FAN_ON, fan_pwm_level);
	} 
	// CMD get_clock;
	////////////////////////////////////////////////////////////////////////////////////
	else if (uart::getInstance()->isEqual(cmd, (char *)CMD_GET_CLOCK, CMD_GET_CLOCK_LEN, cmdLength)){
		uart::getInstance()->TransmitString("\nCMD_GET_CLOCK ");
		uart::getInstance()->TransmitString(getInstance()->clock->getCurrentTime().toString());
	} 
	// CMD set_clock_33_03_03_3_30_03_2033; // set_clock_21_01_08_2_14_03_2017;
	////////////////////////////////////////////////////////////////////////////////////
	else if (uart::getInstance()->isEqual(cmd, (char *)CMD_SET_CLOCK, cmdLength - CMD_SET_CLOCK_ARG_LEN, CMD_SET_CLOCK_LEN, cmdLength)){
		uart::getInstance()->TransmitString("\nCMD_SET_CLOCK ");

		datetime_t datetime = datetime_t(
			(uint8_t)stringToUint16(cmd + (CLOCK_SECOND_OFFSET), CLOCK_SECOND_OFFSET_LEN),
			(uint8_t)stringToUint16(cmd + (CLOCK_MINUTE_OFFSET), CLOCK_MINUTE_OFFSET_LEN),
			(uint8_t)stringToUint16(cmd + (CLOCK_HOUR_OFFSET), CLOCK_HOUR_OFFSET_LEN),
			(uint8_t)stringToUint16(cmd + (CLOCK_DAY_OFFSET), CLOCK_DAY_OFFSET_LEN),
			(uint8_t)stringToUint16(cmd + (CLOCK_DATE_OFFSET), CLOCK_DATE_OFFSET_LEN),
			(uint8_t)stringToUint16(cmd + (CLOCK_MONTH_OFFSET), CLOCK_MONTH_OFFSET_LEN),
			stringToUint16(cmd + (CLOCK_YEAR_OFFSET), CLOCK_YEAR_OFFSET_LEN));
		getInstance()->clock->setTime(datetime);
	} 
	// CMD get_temp;
	////////////////////////////////////////////////////////////////////////////////////
	else if (uart::getInstance()->isEqual(cmd, (char *)CMD_GET_TEMP, cmdLength - CMD_GET_TEMP_ARG_LEN, 
		CMD_GET_TEMP_LEN, cmdLength)) {
		uint8_t temp = 0;
		uart::getInstance()->TransmitString("\nCMD_GET_TEMP_");
		if(cmd[cmdLength - CMD_GET_TEMP_ARG_LEN] == '0'){
			temp = getInstance()->baro_inside.ReadTemperatureRound();
			// uint8_t temp = getInstance()->getTemperature(INSIDE_BAROMETER_ID); // <-- Deze fuckst
		}else if (cmd[cmdLength - CMD_GET_TEMP_ARG_LEN] == '1'){
			temp = getInstance()->baro_outside.ReadTemperatureRound();
		}else {
			return;
		}
		uart::getInstance()->Transmit(temp);
	}else if (uart::getInstance()->isEqual(cmd, (char *)CMD_GET_PRESS, cmdLength - CMD_GET_PRESS_ARG_LEN, 
		CMD_GET_PRESS_LEN, cmdLength)) {
		uart::getInstance()->TransmitString("\nCMD_GET_PRESS_");
		if(cmd[cmdLength - CMD_GET_PRESS_ARG_LEN] == '0'){
			sprintf(string, "pressure is %ld\n", getInstance()->baro_inside.ReadPressure());
		}else if (cmd[cmdLength - CMD_GET_PRESS_ARG_LEN] == '1'){
			sprintf(string, "pressure is %ld\n", getInstance()->baro_outside.ReadPressure());
		}else {
			return;
		}
		uart::getInstance()->TransmitString(string);
	}
	getInstance()->updateHardware();
}


