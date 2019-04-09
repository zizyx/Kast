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



// class climateControl;

// Callback function type definition.
// f_callback_t is a type: fiunction pointer taking a char* and uint8_t, returning void)
typedef void (*f_rxCallback_t)(char *, uint8_t);

class uart {
	public:
		uart(void);
		void print(const char *string);
		void print(const char *string, uint8_t len);
		void Transmit(uint8_t data);
		void checkBuffer();
		void setCallback(f_rxCallback_t);

	private:
		void Init(uint16_t baudrate);
		uint8_t decodeBuffer(char *rxBuffer, uint8_t len);

		/* Will report the bytes received on the rx line. */
		f_rxCallback_t m_rxCallback;
};

#endif