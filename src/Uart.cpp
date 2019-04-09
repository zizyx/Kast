/////////////////////////////////////////////////////////////////////////////////
// UART
/////////////////////////////////////////////////////////////////////////////////
#include "uart.h"
#include "climateControl.h"


static char rxBuffer[RX_BUFFER_SIZE];
volatile static uint8_t bufferLength = 0;

uart::uart(){
	Init(BAUD_RATE);
}

void uart::Init(uint16_t baudrate){

	m_rxCallback = NULL;

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

void uart::print(const char *string, uint8_t len){
	for(uint8_t i = 0; len > i; i++){
		Transmit(string[i]);
	}
}

void uart::print(const char *string){
	uint8_t len = strlen(string);
	print(string, len);
}

void uart::Transmit(uint8_t data){
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)) );

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

/* If the caller returns and need the data to be saved, it needs to copy the values */
void uart::setCallback(f_rxCallback_t cb) {
	m_rxCallback = cb;
}

void uart::checkBuffer()
{
	if (m_rxCallback == NULL)
		return;

	cli();
	m_rxCallback(rxBuffer, bufferLength);
	sei();

	//Clear buffer
	bufferLength = 0;
}

ISR(USART_RX_vect){
	if(UCSR0A & (1<<RXC0)) {
		// bufferLength = (bufferLength + 1 ) % RX_BUFFER_SIZE;
		rxBuffer[bufferLength++] = UDR0;

		// This makes it a circulair buffer which still accepts
		// new bytes to be added, even if it is filled with
		// garbage.
		bufferLength = (bufferLength % RX_BUFFER_SIZE);
	}
}
