#ifndef BOARD_H
#define BOARD_H

#include "adc.h"

//Arduino A0
#define HYGROMETER_ACD_CHANNEL 	CHANNEL_0

//Arduino digital pin 11 //PB3
#define FAN_PIN					PINB3

//Arduino analog pin 4
#define SDA						NULL
//Arduino analog pin 5
#define SCL						NULL

#define COLD_LAMP_PIN			PIND2
#define WARM_LAMP_PIN 			PIND7

//PB4 arduino pin 12
#define WATER_PUMP_PIN 			PIND4

#define OUTPUT					1
#define INPUT 					0
#define HIGH					1
#define LOW						0
#define PULL_UP					1
#define NO_PULL					0

#define ASCII_OFFSET			'0'

#endif //BOARD_H