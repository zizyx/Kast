/////////////////////////////////////////////////////////////////////////////////
// UART
/////////////////////////////////////////////////////////////////////////////////
#include "uart.h"
#include "climateControl.h"

static char rxBuffer[RX_BUFFER_SIZE];
volatile static uint8_t bufferLength = 0;

uart uart::instance = uart();

uart::uart(){
	Init(BAUD_RATE);
}

uart *uart::getInstance(){
	return &instance;
}

void uart::Init(uint16_t baudrate){
	/*Set baud rate */
	UBRR0H = (baudrate >> 8);
	UBRR0L = baudrate;

	//Enable receiver and transmitter 
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);

	// enable uart interrupt
	UCSR0B |= (1 << RXCIE0);
	sei();
}

void uart::print(char *string, uint8_t len){
	TransmitString(string, len);
}

void uart::print(char string[]){
	uint8_t len = strlen(string);
	TransmitString(string, len);
}

void uart::Transmit(uint8_t data){
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)) );

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void uart::TransmitString(char *string, uint8_t len){
	for(uint8_t i = 0; len > i; i++){
		Transmit(string[i]);
	}
}

void uart::checkBuffer(climateControl *cctl)
{
	uint8_t new_len;

	for(uint8_t i = 0; i < bufferLength; i++){
		if(rxBuffer[i] == FLUSH){ //flush buffer after sending data
			new_len = decodeBuffer(rxBuffer, i);
			cctl->handleCmd(rxBuffer, new_len);
			bufferLength = 0;
		}
	}
	// print(rxBuffer + '\0');
}

#define END FLUSH
#define ESC '|'
#define ESC_ESC '|'
#define ESC_END '+'

uint8_t uart::decodeBuffer(char *rxBuffer, uint8_t len) {
		uint8_t byte, new_len;
		bool escaping = false;
		char *buffer;
		uint8_t i, j, nr_escaped;

		buffer = (char *)calloc(len, sizeof(char));
		new_len = 0;
		nr_escaped = 0;

		for (i = 0, j = 0; i < len; i++, j++) {
			byte = rxBuffer[i];

			if (escaping == false) {
				if (byte == ESC) {
					escaping = true;
					continue;
				}
			} else {
				nr_escaped++;
				escaping = false;
				if (byte == ESC_END) {
					buffer[j] = END;
					continue;
				}

				if (byte == ESC_ESC) {
					buffer[j] = ESC;
					continue;
				}
			}
			buffer[j] = byte;
		}
		// memcpy(rxBuffer, buffer, len - nr_escaped);

		free(buffer);

		return len;
}


bool uart::isEqual(char *a, char *b, uint8_t validateLength, uint8_t cmdLength, uint8_t length){
	if(length != cmdLength){
		// TransmitString("length != cmdLength'\n");
		// TransmitString("length = "); Transmit(length); TransmitString("\n");
		// TransmitString("cmdLength = "); Transmit(cmdLength); TransmitString("\n");
		return false;
	}

	for(uint8_t i = 0; i < validateLength; i++){
		if(a[i] != b[i]){
			// TransmitString("a[i] != b[i]\n");
			return false;
		}
	}
	// TransmitString("Equal :D\n");
	return true; //Equal :D
}

bool uart::isEqual(char *a, char *b, uint8_t length, uint8_t cmdLength){
	return isEqual(a, b, length, cmdLength, length);	
}

ISR(USART_RX_vect){
	if(UCSR0A & (1<<RXC0)) {
		rxBuffer[bufferLength++] = UDR0;
	}
}
