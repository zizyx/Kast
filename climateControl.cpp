#include <math.h>
#include "plants.h"
#include "nvm_store.h"
#include "climateControl.h"

char string[81] = {"e\n\0"};

uint16_t stringToUint16(char* str, uint8_t str_len) {
	double val = 0;
	for (int i = 0, j = str_len - 1; i < str_len; i++, j--) {
		val += (str[i] - '0') * pow(10, j);
		// m_serial.Transmit('\n'); //Echo what is sent to the device	
		// m_serial.Transmit(str[i] - '0'); //Echo what is sent to the device	
		// m_serial.TransmitString(" --- "); //Echo what is sent to the device	
		// m_serial.Transmit(val); //Echo what is sent to the device	
	}
	return round(val);
}

climateControl::climateControl(i2c &twi, uart &serialInterface, DS_3231 &clock) : 
    m_baro_inside(BMP280_ADDRESS_INSIDE, twi),
    m_baro_outside(BMP280_ADDRESS_OUTSIDE, twi),
    m_serial(serialInterface),
    m_clock(clock)
{
	struct plant_t plant;
	uint8_t plant_crc, plant_nvm_crc;

//	setPlantToGrow(KUSH);
	adc Adc;
	nvm Nvm;

	vars.inside_temp = 0;
	vars.outside_temp = 0;

	vars.inside_pressure = 0;
	vars.outside_pressure = 0;

	vars.fan_status = FAN_ON;
	vars.fan_pwm_level = 0;

	vars.lamp_status[LAMPS] = LAMP_OFF;
	vars.lamp_time[LAMPS] = LAMP_OFF;

	vars.water_pump_status = WATER_PUMP_OFF;
	vars.water_pump_time = 0;

	vars.humidity = 0;

	pdm Pdm;
	Pdm.setupFanPin(FAN_PIN);
	Pdm.setupPinTimer();
	Pdm.setPdm(255);

	setupLampHardware();
	setupWaterPumpHardware();

	updateHardware();

	// vars.selected_plant = plants[NO_PLANT];

	Nvm.nvmReadBlock(PLANT_OFFSET, (uint8_t *)&plant, PLANT_SIZE);
	plant_crc = Nvm.calcCrc((uint8_t *)&plant, PLANT_SIZE);
	plant_nvm_crc = Nvm.nvmRead(PLANT_CRC_OFFSET);

	char str[80];
	sprintf(str, "Plant_crc %u, plant_nvm_crc %u \n", plant_crc, plant_nvm_crc);
	// PRINT_STR(str);

	if (plant_nvm_crc != plant_crc) {
		// PRINT_STR("Plant crc in nvm is incorrect.\n");

		plant.id = INVALID_PLANT_ID;
		plant.growing_started = false;
		Nvm.nvmWriteBlock(PLANT_OFFSET, (uint8_t *)&plant, PLANT_SIZE);

		plant_crc = Nvm.calcCrc((uint8_t *)&plant, PLANT_SIZE);
		Nvm.nvmWrite(PLANT_CRC_OFFSET, plant_crc);
	} else if (plant.id != INVALID_PLANT_ID) {
		// PRINT_STR("Loaded plant from nvm.\n");
		//Dan shit uitlezen en doen wat moet. Le maneure is prima.
	} else {
		// PRINT_STR("Unkown plant set.\n");
		//INVALID plant maar crc klopt.
		//printje plant is not set.
	}

}

//Configure the pins as: output, no_pull, driven low
void climateControl::setupLampHardware() {
	DDRD |= (1 << COLD_LAMP_PIN) | (1 << WARM_LAMP_PIN); //Output
	PORTD &= ~(1 << COLD_LAMP_PIN) | (1 << WARM_LAMP_PIN); //No_pull
	PIND &= ~(1 << COLD_LAMP_PIN) | (1 << WARM_LAMP_PIN); //Drive low
}

//Configure the pins as: output, no_pull, driven low
void climateControl::setupWaterPumpHardware() {
	DDRD |= (1 << WATER_PUMP_PIN); //Output
	PORTD &= ~(1 << WATER_PUMP_PIN); //No_pull
	PIND &= ~(1 << WATER_PUMP_PIN); //Drive low
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
	// if ( vars.inside_temp >= vars.selected_plant.max_temp ) {
	// 	active = true;
	// 	vars.lamp_status[WARM_LAMP] = LAMP_OFF;
	// 	vars.lamp_status[COLD_LAMP] = LAMP_OFF;	
	// 	setLampState(WARM_LAMP, LAMP_OFF);
	// 	setLampState(COLD_LAMP, LAMP_OFF);
	// 	setFanState(FAN_ON, MAX_FAN_PWM); //Cool that plant. max
	// }

	// if ( vars.humidity >= vars.selected_plant.max_humidity ) {
	// 	active = true;
	// 	setWaterPumpState(WATER_PUMP_OFF);
	// }

	// if ( vars.inside_pressure >= ( vars.inside_pressure + (UPPER_PRESSURE_TOLERANCE * 2) ) ) {
	// 	active = true;
	// 	setFanState(FAN_OFF, 0);
	// }

	return active;
}


//UPDATE
void climateControl::updateClimateVars() {
	// vars.outside_pressure = getPressure(BMP280_ADDRESS_OUTSIDE);
	// vars.outside_temp = getTemperature(BMP280_ADDRESS_OUTSIDE);

	// vars.inside_pressure = getPressure(BMP280_ADDRESS_INSIDE);
	// vars.inside_temp = getTemperature(BMP280_ADDRESS_INSIDE);

	// vars.humidity = getHumidity();
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
	// DEBUG_STR("setFanState");
	// m_serial.Transmit(vars.fan_pwm_level);
}

void climateControl::setWaterPumpState(uint8_t status) {
	
	vars.water_pump_status = status;
	vars.water_pump_time = 0; //Update timer value
}

//Calls functions which calculate and update climate vaiables
void climateControl::calculateClimateVars() {
	calculateFanPwmVars();
	calculateLampVars(WARM_LAMP);
	calculateLampVars(COLD_LAMP);
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

uint16_t climateControl::getHumidity(){
	Adc.setChannel(CHANNEL_ZERO);
	Adc.startFirstConversion();
	_delay_ms(100); 

	// char string[81] = {"e\n\0"};
	// sprintf(string, "Adc result is %d.\n", Adc.readAdc());	
	// PRINT_STR(string);

	return Adc.readAdc();
}

void climateControl::calculateLampVars(uint8_t lamp_id) {
	vars.lamp_status[WARM_LAMP] = LAMP_ON; //or off
	vars.lamp_status[COLD_LAMP] = LAMP_ON; //or off
}

void climateControl::calculateWaterPumpVars() {
	vars.water_pump_status = WATER_PUMP_ON;
}

void climateControl::setPlantToGrow(uint16_t plant_id) {
//	vars.selected_plant = plant_list[plant_id];
}

//HARDWARE setters
void climateControl::updateHardware() {
	// PRINT_STR("update hardware");
	setFanHardware();
	setLampHardware();
	setWaterPumpHardware();
}

void climateControl::setFanHardware() {
	// PRINT_STR("setFanHardware ");
	// DEBUG_STR(vars.fan_pwm_level);
	if (vars.fan_status == FAN_ON){
		Pdm.setPdm(vars.fan_pwm_level);
	} else if (vars.fan_status == FAN_OFF){
		Pdm.setPdm(MIN_FAN_PWM);
	}
	//Zet pwm regs
}

void climateControl::setLampHardware() {
	PIND &= ~(1 << COLD_LAMP_PIN) | (1 << WARM_LAMP_PIN); //Drive low
	PIND |= (vars.lamp_status[COLD_LAMP] << COLD_LAMP_PIN) | 
		(vars.lamp_status[WARM_LAMP] << WARM_LAMP_PIN);	
} 

void climateControl::setWaterPumpHardware() {
	PINB &= ~(1 << WATER_PUMP_PIN); //Drive low
	PINB |= (vars.water_pump_status << COLD_LAMP_PIN);
	//Setup timeout timer.
}

void climateControl::handleCmd(char *cmd, uint8_t cmdLength) {
	// reading incoming string
	for(uint8_t i = 0; i < cmdLength; i++){
		// m_serial.Transmit(cmd[i]); //Echo what is sent to the device	
	}
	
	// CMD fan_on;
	////////////////////////////////////////////////////////////////////////////////////
	if (m_serial.isEqual(cmd, (char *)CMD_FAN_ON, CMD_FAN_ON_LEN, cmdLength)){
		sprintf(string, "Turning fan on.\n");
	 	setFanState(FAN_ON);
	} 
	
	// CMD fan_off;
	////////////////////////////////////////////////////////////////////////////////////
	else if (m_serial.isEqual(cmd, (char *)CMD_FAN_OFF, CMD_FAN_OFF_LEN, cmdLength)){
		sprintf(string, "Turning off fan.\n");
	 	setFanState(FAN_OFF);
	} 

	
	// CMD Fan_lvl_xxx;
	////////////////////////////////////////////////////////////////////////////////////
	else if (m_serial.isEqual(cmd, (char *)CMD_FAN_LVL, cmdLength - CMD_FAN_LVL_ARG_LEN, 
			CMD_FAN_LVL_LEN, cmdLength)) {

		uint8_t fan_pwm_level = 0;
		fan_pwm_level = stringToUint16(cmd + (CMD_FAN_LVL_LEN - CMD_FAN_LVL_ARG_LEN), CMD_FAN_LVL_ARG_LEN);

		sprintf(string, "Updating fan lvl to %d\n", fan_pwm_level);
	 	setFanState(FAN_ON, fan_pwm_level);
	} 
	
	// CMD set_clock_33_03_03_3_30_03_2033; // set_clock_21_01_08_2_14_03_2017;
	////////////////////////////////////////////////////////////////////////////////////
	else if (m_serial.isEqual(cmd, (char *)CMD_SET_CLOCK, cmdLength - CMD_SET_CLOCK_ARG_LEN, CMD_SET_CLOCK_LEN, cmdLength)){
		datetime_t datetime = datetime_t(
			(uint8_t)stringToUint16(cmd + (CLOCK_SECOND_OFFSET), CLOCK_SECOND_OFFSET_LEN),
			(uint8_t)stringToUint16(cmd + (CLOCK_MINUTE_OFFSET), CLOCK_MINUTE_OFFSET_LEN),
			(uint8_t)stringToUint16(cmd + (CLOCK_HOUR_OFFSET), CLOCK_HOUR_OFFSET_LEN),
			(uint8_t)stringToUint16(cmd + (CLOCK_DAY_OFFSET), CLOCK_DAY_OFFSET_LEN),
			(uint8_t)stringToUint16(cmd + (CLOCK_DATE_OFFSET), CLOCK_DATE_OFFSET_LEN),
			(uint8_t)stringToUint16(cmd + (CLOCK_MONTH_OFFSET), CLOCK_MONTH_OFFSET_LEN),
			stringToUint16(cmd + (CLOCK_YEAR_OFFSET), CLOCK_YEAR_OFFSET_LEN));
		m_clock.setTime(datetime);
		sprintf(string, "Updated datetime to %s\n", m_clock.getCurrentTime().toString());
	} 

	// CMD get_clock;
	////////////////////////////////////////////////////////////////////////////////////
	else if (m_serial.isEqual(cmd, (char *)CMD_GET_CLOCK, CMD_GET_CLOCK_LEN, cmdLength)){
		datetime_t tempTime = m_clock.getCurrentTime();


		sprintf(string, "Current datetime is %s\n", tempTime.toString());
	} 

	// CMD get_temp;
	////////////////////////////////////////////////////////////////////////////////////
	else if (m_serial.isEqual(cmd, (char *)CMD_GET_TEMP, cmdLength - CMD_GET_TEMP_ARG_LEN, 
		CMD_GET_TEMP_LEN, cmdLength)) {
		if (cmd[cmdLength - CMD_GET_TEMP_ARG_LEN] == '0'){
			sprintf(string, "temp is %d\n", m_baro_inside.ReadTemperatureRound()); //Deze is weer genaaid
			// sprintf(string, "temp is %d\n", 10); //Deze is weer genaaid
		} else if (cmd[cmdLength - CMD_GET_TEMP_ARG_LEN] == '1'){
			sprintf(string, "temp is %d\n", m_baro_outside.ReadTemperatureRound()); //Deze is weer genaaid
			// sprintf(string, "temp is %d\n", 20); //Deze is weer genaaid
		} else {
			return;
		}
	}

	// CMD get_press;
	////////////////////////////////////////////////////////////////////////////////////		
	else if (m_serial.isEqual(cmd, (char *)CMD_GET_PRESS, cmdLength - CMD_GET_PRESS_ARG_LEN, 
		CMD_GET_PRESS_LEN, cmdLength)) {
		if (cmd[cmdLength - CMD_GET_PRESS_ARG_LEN] == '0'){
			sprintf(string, "pressure is %ld Pascal\n", m_baro_inside.ReadPressure()); //Deze is weer genaaid
			// sprintf(string, "pressure is %ld Pascal\n", 11L); //Deze is weer genaaid
		} else if (cmd[cmdLength - CMD_GET_PRESS_ARG_LEN] == '1'){
			sprintf(string, "pressure is %ld Pascal\n", m_baro_outside.ReadPressure()); //Deze is weer genaaid
			// sprintf(string, "pressure is %ld Pascal\n", 22L); //Deze is weer genaaid
		} else {
			return;
		}
	}

	// CMD get_humid;
	////////////////////////////////////////////////////////////////////////////////////		
	else if (m_serial.isEqual(cmd, (char *)CMD_GET_HUMID, CMD_GET_HUMID_LEN, cmdLength)){
		sprintf(string, "Humidity is %d.\n", getHumidity());
		// Around 500+ is dry
		// Fully wet is around 300-
		// Test in ground to be sure
	}

	// CMD set_warm_lamp_;
	////////////////////////////////////////////////////////////////////////////////////
	else if (m_serial.isEqual(cmd, (char *)CMD_SET_WARM_LAMP, cmdLength - CMD_SET_WARM_LAMP_ARG_LEN, 
		CMD_SET_WARM_LAMP_LEN, cmdLength)) {
		if (cmd[cmdLength - CMD_SET_WARM_LAMP_ARG_LEN] == '0'){
			sprintf(string, "Turning warm lamp off\n");
			setLampState(WARM_LAMP, LAMP_OFF);
		} else if (cmd[cmdLength - CMD_SET_WARM_LAMP_ARG_LEN] == '1'){
			sprintf(string, "Turning warm lamp on\n");
			setLampState(WARM_LAMP, LAMP_ON);
		} else {
			return;
		}
	}
	// CMD set_cold_lamp_;
	////////////////////////////////////////////////////////////////////////////////////
	else if (m_serial.isEqual(cmd, (char *)CMD_SET_COLD_LAMP, cmdLength - CMD_SET_COLD_LAMP_ARG_LEN, 
		CMD_SET_COLD_LAMP_LEN, cmdLength)) {
		if (cmd[cmdLength - CMD_SET_COLD_LAMP_ARG_LEN] == '0'){
			sprintf(string, "Turning cold lamp off\n");
			setLampState(COLD_LAMP, LAMP_OFF);
		} else if (cmd[cmdLength - CMD_SET_COLD_LAMP_ARG_LEN] == '1'){
			sprintf(string, "Turning cold lamp on\n");
			setLampState(COLD_LAMP, LAMP_ON);
		} else {
			return;
		}
	}
	// CMD set_water_pump_;
	////////////////////////////////////////////////////////////////////////////////////
	else if (m_serial.isEqual(cmd, (char *)CMD_SET_WATER_PUMP, cmdLength - CMD_SET_WATER_PUMP_ARG_LEN, 
		CMD_SET_WATER_PUMP_LEN, cmdLength)) {
		if (cmd[cmdLength - CMD_SET_WATER_PUMP_ARG_LEN] == '0'){
			sprintf(string, "Turning water pump off\n");
			setWaterPumpState(WATER_PUMP_OFF);
		} else if (cmd[cmdLength - CMD_SET_WATER_PUMP_ARG_LEN] == '1'){
			sprintf(string, "Turning water pump on\n");
			setWaterPumpState(WATER_PUMP_ON);
		} else {
			return;
		}
	}
	m_serial.print(string);
	return;

	// CMD read_nvm_;
	////////////////////////////////////////////////////////////////////////////////////
	if (m_serial.isEqual(cmd, (char *)CMD_READ_NVM, cmdLength - CMD_READ_NVM_ARG_LEN, 
		CMD_READ_NVM_LEN, cmdLength)) {
		if (cmd[cmdLength - CMD_READ_NVM_ARG_LEN] == '0'){
			sprintf(string, "Reading NVM Address\n");

			uint8_t nvmBuffer[50];
			uint16_t startAddress = stringToUint16(cmd + NVM_START_ADDRESS_OFFSET, NVM_ADDRESS_LEN);
			uint8_t dataLength = (uint8_t)stringToUint16(cmd + NVM_DATA_LENGTH_OFFSET, NVM_DATA_LEN);

			if(dataLength <= 50){
				Nvm.nvmReadBlock(startAddress, nvmBuffer, dataLength);
			}

			sprintf(string, "Start Adress: %d, Data length: %d, Data: ", startAddress, dataLength);
			// PRINT_STR(string);
			// PRINT_STR_LEN((char *)nvmBuffer, dataLength);
			return;
		} else {
			return;
		}
	}
	// CMD write_nvm_;
	////////////////////////////////////////////////////////////////////////////////////
	// } else if (m_serial.isEqual(cmd, (char *)CMD_WRITE_NVM, cmdLength - CMD_WRITE_NVM_ARG_LEN, 
	// 	CMD_WRITE_NVM_LEN, cmdLength)) {

// #define END FLUSH
// #define ESC '|'
// #define ESC_ESC '|'
// #define ESC_END '+'

// 		uint8_t data_len, byte;
// 		bool escaping = false;
// 		uint8_t buffer[50];

// 		for (uint8_t i = NVM_DATA_LENGTH_OFFSET, j = 0; i < RX_BUFFER_SIZE - 1; i++, j++) {
// 			byte = (cmd + i);

// 			if (escaping == false) {
// 				if (byte == ESC) {
// 					escaping = true;
// 					continue;
// 				}

// 				if (byte == END) {
// 					break;
// 				}
// 			} else {
// 				escaping = false;
// 				if (byte == ESC_END) {
// 					buffer[j] = END;
// 					continue;
// 				}
// 			}


// 			buffer[j] = byte;		
// 		}


// 		if (cmd[cmdLength - CMD_WRITE_NVM_ARG_LEN] == '0'){
// 			sprintf(string, "Writing NVM Address\n");
// 			// setWaterPumpStREAD(NVM_OFF);
// 		} else {
// 			return;
// 		}
	// }
	// PRINT_STR(string);
	// updateHardware();
}
