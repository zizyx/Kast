#ifndef UART_H
#define UART_H

#define BAUD_9600			103
#define BAUD_RATE			BAUD_9600
#define FLUSH 				';'
#define ESC 				'|'
#define ESC_ESC '|'
#define ESC_END '+'
#define END FLUSH
#define RX_BUFFER_SIZE 		50
#define IS_CALLBACK_SET		"IS_CALLBACK_SET"


#include <inttypes.h> // includes stdint
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>

// #define ENABLE_DEBUG
// #ifdef ENABLE_DEBUG
// #define DEBUG_STR(x)			(PRINT_STR(x))
// #else
// #define DEBUG_STR(x)			()
// #endif

class climateControl;

//  AmemberFunc points to a member of A that takes ()
typedef  void (climateControl::*callback)(char *, uint8_t);

class uart {
	public:
		uart(void);
		void print(char *string);
		void print(char *string, uint8_t len);
		void Transmit(uint8_t data);
		void TransmitString(char *string, uint8_t len);
		void checkBuffer(climateControl &c, callback cb);
		bool isEqual(char *a, char *b, uint8_t length, uint8_t cmdLength);
		bool isEqual(char *a, char *b, uint8_t length, uint8_t cmdLength, uint8_t equalLength);

	private:
		void Init(uint16_t baudrate);
		uint8_t decodeBuffer(char *rxBuffer, uint8_t len);
};

#endif