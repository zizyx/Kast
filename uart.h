#ifndef UART_H
#define UART_H

#define BAUD_RATE			103
#define FLUSH 				';'
#define RX_BUFFER_SIZE 		50
#define IS_CALLBACK_SET		"IS_CALLBACK_SET"

#include <inttypes.h> // includes stdint
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include "uartCallback.h"
#include "climateControl.h"

#define PRINT_STR(o, x)	(o->print((char *) x))

#define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#define DEBUG_STR(x)	(uart::getInstance()->print((char *) x))
#else
#define DEBUG_STR(x)	()
#endif

class climateControl;

class uart {
	public:
		uart(void);
		static uart *getInstance();
		void print(char string[]);
		void Transmit(uint8_t data);
		void TransmitString(char string[]);
		void checkBuffer(climateControl *cctl);
		bool isEqual(char *a, char *b, uint8_t length, uint8_t cmdLength);
		bool isEqual(char *a, char *b, uint8_t length, uint8_t cmdLength, uint8_t equalLength);

	    // void connectCallback(uartCallback *cb)
	    // {
	    //     m_cb = cb;
	    // }

	private:
		static uart instance;
    	// uartCallback *m_cb;
		void Init(uint16_t baudrate);
};

#endif