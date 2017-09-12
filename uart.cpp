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

void uart::print(char string[]){
	TransmitString(string);
}

void uart::Transmit(uint8_t data){
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)) );

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void uart::TransmitString(char string[]){
	uint8_t len = strlen(string);
	uint8_t i;

	for(i = 0; len > i; i++){
		Transmit(string[i]);	    
	}
}

void uart::checkBuffer(climateControl *cctl)
{
	for(uint8_t i = 0; i < bufferLength; i++){
		if(rxBuffer[i] == FLUSH){ //flush buffer after sending data
			cctl->handleCmd(rxBuffer, i);
			bufferLength = 0;
		}
	}
	// TransmitString(rxBuffer);
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
