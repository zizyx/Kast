#ifndef UART_H
#define UART_H

#define BAUD_9600			103
#define BAUD_RATE			BAUD_9600
#define FLUSH 				';'
#define END FLUSH
#define ESC 				'|'
#define RX_BUFFER_SIZE 		50
#define IS_CALLBACK_SET		"IS_CALLBACK_SET"

#include <inttypes.h> // includes stdint
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>

#include "climateControl.h"

#define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#define DEBUG_STR(x)			(PRINT_STR(x))
#else
#define DEBUG_STR(x)			()
#endif

#define PRINT_STR(x)			(uart::getInstance()->print((char *) x))
#define PRINT_STR_LEN(x, len)	(uart::getInstance()->print((char *) x, len))

class climateControl;

class uart {
	public:
		static uart *getInstance();
		void print(char string[]);
		void print(char *string, uint8_t len);
		void Transmit(uint8_t data);
		void TransmitString(char *string, uint8_t len);
		void checkBuffer(climateControl *cctl);
		bool isEqual(char *a, char *b, uint8_t length, uint8_t cmdLength);
		bool isEqual(char *a, char *b, uint8_t length, uint8_t cmdLength, uint8_t equalLength);

	private:
		uart(void);
		static uart instance;
		void Init(uint16_t baudrate);
		uint8_t decodeBuffer(char *rxBuffer, uint8_t len);
};

#endif