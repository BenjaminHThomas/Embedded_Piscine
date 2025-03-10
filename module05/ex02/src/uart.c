#include "uart.h"

void uart_init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// set frame format: 8data
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	// UCSR0B |= (1 << RXCIE0); // enable rx interrupt
}

void uart_tx(char c) {
	while (!(UCSR0A & (1<<UDRE0))) ;
	UDR0 = c;
}

void uart_tx_str(char *str) {
	if (!str)
		return ;
	while (*str) {
		uart_tx(*str++);
	}
}

char uart_rx(void) {
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void backspace(void) {
	uart_tx(BACKSPACE);
	uart_tx(' ');
	uart_tx(BACKSPACE); 
}

void get_uinput(char * buff, unsigned int maxSize) {
	char c = 0;
	for (uint8_t i = 0; i < maxSize; i++){
		buff[i] = 0;
	}
	uint8_t i = 0;
	while (c != ENTERKEY) {
		c = uart_rx();
		if (i < maxSize) {
			buff[i++] = c;
			uart_tx(c);
		}
		if ((c == BACKSPACE || c == DELETE ) && i > 0) {
			backspace();
			buff[--i] = 0;
		}
	}
	uart_tx('\r');
	uart_tx('\n');
}
