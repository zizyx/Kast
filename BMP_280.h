#ifndef BMP_280_H
#define BMP_280_H

#include <inttypes.h> //includes stdint
#include "i2c.h"

#define BMP280_START_MEASUREMENT			 0b01001011

#define BMP280_REGISTER_DIG_T1               0x88 
#define BMP280_REGISTER_DIG_T2             	 0x8A
#define BMP280_REGISTER_DIG_T3               0x8C

#define BMP280_REGISTER_DIG_P1               0x8E
#define BMP280_REGISTER_DIG_P2               0x90
#define BMP280_REGISTER_DIG_P3               0x92
#define BMP280_REGISTER_DIG_P4               0x94
#define BMP280_REGISTER_DIG_P5               0x96
#define BMP280_REGISTER_DIG_P6               0x98
#define BMP280_REGISTER_DIG_P7               0x9A
#define BMP280_REGISTER_DIG_P8               0x9C
#define BMP280_REGISTER_DIG_P9               0x9E

#define BMP280_REGISTER_CTRL_MEAS		   	 0xF4

#define BMP280_REGISTER_TEMP_MSB		   	 0xFA
#define BMP280_REGISTER_TEMP_LSB		   	 0xFB
#define BMP280_REGISTER_TEMP_XLSB		   	 0xFC

#define BMP280_REGISTER_PRESSURE_MSB	   	 0xF7
#define BMP280_REGISTER_PRESSURE_LSB	   	 0xF8
#define BMP280_REGISTER_PRESSURE_XLSB	   	 0xF9

#define TEST_REGISTER						 0xD0
#define ADDRESS_LOW			 			 	 0x76
#define ADDRESS_HIGH		 				 0x77

class BMP_280{
	public:
		BMP_280(uint8_t address);
		uint32_t ReadTemperature();
		uint8_t ReadTemperatureRound();
		uint32_t ReadPressure();
		uint8_t ReadPressureRound();
		struct bmp280data {
			uint8_t address;
			uint8_t msb_temp;
			uint8_t lsb_temp;
			uint8_t xlsb_temp;

			uint8_t msb_pres;
			uint8_t lsb_pres;
			uint8_t xlsb_pres;

			uint16_t digt1;
			int16_t digt2;
			int16_t digt3;

			uint16_t digp1;
			int16_t digp2;
			int16_t digp3;
			int16_t digp4;
			int16_t digp5;
			int16_t digp6;
			int16_t digp7;
			int16_t digp8;
			int16_t digp9;

			int32_t t_fine;
			uint32_t calculated_tmp;
			int64_t calculated_pressure;
		} bmp280_data;
	private:
		i2c *I2C;
		void ReadPressureRegisters();
		void ReadTempRegisters();

		uint32_t Calc(void);
		void PressureCalc();
		void TempCalc();
};

#endif