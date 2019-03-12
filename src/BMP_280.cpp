/////////////////////////////////////////////////////////////////////////////////
// BMP280 - Barometer
/////////////////////////////////////////////////////////////////////////////////
#include "BMP_280.h"

BMP_280::BMP_280(uint8_t address, i2c &twi) :
	m_i2c(twi)
{
	bmp280_data.address = address;
}

uint32_t BMP_280::ReadTemperature(){
	ReadTempRegisters();
	TempCalc();
	return bmp280_data.calculated_tmp;
}

uint8_t BMP_280::ReadTemperatureRound() {
	return (uint8_t)(ReadTemperature() / 100);
}

bool BMP_280::ReadTempRegisters(){
	// Start command
	
	if (m_i2c.WriteData(bmp280_data.address, BMP280_REGISTER_CTRL_MEAS, BMP280_START_MEASUREMENT, true) == false)
		return false;			

	if (m_i2c.ReadRegisteru8(bmp280_data.address, BMP280_REGISTER_TEMP_MSB, &bmp280_data.msb_temp) == false)
		return false;

	if (m_i2c.ReadRegisteru8(bmp280_data.address, BMP280_REGISTER_TEMP_LSB, &bmp280_data.lsb_temp) == false)
		return false;

	if (m_i2c.ReadRegisteru8(bmp280_data.address, BMP280_REGISTER_TEMP_XLSB, &bmp280_data.xlsb_temp) == false)
		return false;

	if (m_i2c.ReadRegisteru16LE(bmp280_data.address, BMP280_REGISTER_DIG_T1, &bmp280_data.digt1) == false)
		return false;

	if (m_i2c.ReadRegisters16LE(bmp280_data.address, BMP280_REGISTER_DIG_T2, &bmp280_data.digt2) == false)
		return false;

	if (m_i2c.ReadRegisters16LE(bmp280_data.address, BMP280_REGISTER_DIG_T3, &bmp280_data.digt3) == false)
		return false;

	return true;
}

void BMP_280::TempCalc(void) {
	int32_t var1, var2, adc_t;

	adc_t = bmp280_data.msb_temp;
	adc_t <<= 8;
	adc_t |= bmp280_data.lsb_temp;
	adc_t <<= 8;
	adc_t |= bmp280_data.xlsb_temp;

	adc_t >>= 4;

	var1 = ((((adc_t >> 3) - ((int32_t)bmp280_data.digt1 << 1))) * 
		((int32_t)bmp280_data.digt2)) >> 11;
	var2 = (((((adc_t>>4) - ((int32_t)bmp280_data.digt1)) * 
		((adc_t>>4) - ((int32_t)bmp280_data.digt1))) >> 12) * 
	((int32_t)bmp280_data.digt3)) >> 14;

	bmp280_data.t_fine = var1 + var2;

	int32_t T = (bmp280_data.t_fine * 5 + 128) >> 8;

	bmp280_data.calculated_tmp = T;
}

uint32_t BMP_280::ReadPressure(){
	ReadPressureRegisters();
	PressureCalc();
	
	return bmp280_data.calculated_pressure;
}

uint8_t BMP_280::ReadPressureRound() {
	return (uint8_t)(ReadPressure() / 100);
}

bool BMP_280::ReadPressureRegisters(){
	if (m_i2c.WriteData(bmp280_data.address, BMP280_REGISTER_CTRL_MEAS, BMP280_START_MEASUREMENT, true) == false)
		return false;	

	if (m_i2c.ReadRegisteru8(bmp280_data.address, BMP280_REGISTER_PRESSURE_MSB, &bmp280_data.msb_pres) == false)
		return false;
	if (m_i2c.ReadRegisteru8(bmp280_data.address, BMP280_REGISTER_PRESSURE_LSB, &bmp280_data.lsb_pres) == false)
		return false;
	if (m_i2c.ReadRegisteru8(bmp280_data.address, BMP280_REGISTER_PRESSURE_XLSB, &bmp280_data.xlsb_pres) == false)
		return false;

	if (m_i2c.ReadRegisteru16LE(bmp280_data.address, BMP280_REGISTER_DIG_P1, &bmp280_data.digp1) == false)
		return false;
	if (m_i2c.ReadRegisters16LE(bmp280_data.address, BMP280_REGISTER_DIG_P2, &bmp280_data.digp2) == false)
		return false;
	if (m_i2c.ReadRegisters16LE(bmp280_data.address, BMP280_REGISTER_DIG_P3, &bmp280_data.digp3) == false)
		return false;
	if (m_i2c.ReadRegisters16LE(bmp280_data.address, BMP280_REGISTER_DIG_P4, &bmp280_data.digp4) == false)
		return false;
	if (m_i2c.ReadRegisters16LE(bmp280_data.address, BMP280_REGISTER_DIG_P5, &bmp280_data.digp5) == false)
		return false;
	if (m_i2c.ReadRegisters16LE(bmp280_data.address, BMP280_REGISTER_DIG_P6, &bmp280_data.digp6) == false)
		return false;
	if (m_i2c.ReadRegisters16LE(bmp280_data.address, BMP280_REGISTER_DIG_P7, &bmp280_data.digp7) == false)
		return false;
	if (m_i2c.ReadRegisters16LE(bmp280_data.address, BMP280_REGISTER_DIG_P8, &bmp280_data.digp8) == false)
		return false;
	if (m_i2c.ReadRegisters16LE(bmp280_data.address, BMP280_REGISTER_DIG_P9, &bmp280_data.digp9) == false)
		return false;

	return true;
}


void BMP_280::PressureCalc() {
	int64_t var1, var2, p;
	int32_t adc_P;

	// Must be done first to get the t_fine variable set up
	ReadTemperature();

	adc_P = bmp280_data.msb_pres;
	adc_P <<= 8;
	adc_P |= bmp280_data.lsb_pres;
	adc_P <<= 8;
	adc_P |= bmp280_data.xlsb_pres;

	adc_P >>= 4;

	var1 = ((int64_t)bmp280_data.t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)bmp280_data.digp6;
	var2 = var2 + ((var1*(int64_t)bmp280_data.digp5)<<17);
	var2 = var2 + (((int64_t)bmp280_data.digp4)<<35);
	var1 = ((var1 * var1 * (int64_t)bmp280_data.digp3)>>8) +
		   ((var1 * (int64_t)bmp280_data.digp2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)bmp280_data.digp1)>>33;

	if (var1 == 0) {
		return;  // avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p<<31) - var2)*3125) / var1;
	var1 = (((int64_t)bmp280_data.digp9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)bmp280_data.digp8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280_data.digp7)<<4);
	bmp280_data.calculated_pressure = p/256;

}