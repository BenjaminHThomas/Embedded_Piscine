// • The program should write ’Z’ to the serial port at 1Hz (do as you wish).

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR F_CPU/16/BAUD-1
// MYUBRR = 8

void uart_tx(char c);

void uart_init(unsigned int baud);

int main(void) {
	uart_init(8);
	while (1){
		uart_tx('Z');
		_delay_ms(1000);
		;
	}
	return 0;
}

void uart_init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// set frame format: 8data
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}

void uart_tx(char c) {
	while (!(UCSR0A & (1<<UDRE0))) ;
	UDR0 = c;
}