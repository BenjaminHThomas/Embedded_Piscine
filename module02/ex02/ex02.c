// Now you will have to write a function uart_rx that waits to receive a character on
// the MCU’s UART port and then returns it.
// • You must write a program that uses your uart_rx function.
// • It should write the received characters from uart_rx to the serial port using your
// uart_tx function (ex00)

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR F_CPU/16/(BAUD-1)
#define PRESCALER 1024
#define COMPARE F_CPU/PRESCALER
#define ENTERKEY 13

void uart_tx(char c) {
	while (!(UCSR0A & (1<<UDRE0))) ;
	UDR0 = c;
}

char uart_rx(void) {
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void uart_init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// set frame format: 8data
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}

int main(void) {
	uart_init(MYUBRR);
	while (1) {
		char c = uart_rx();
		if (c == ENTERKEY) {
			uart_tx('\r');
			uart_tx('\n');
		} else
			uart_tx(c);
	}
	return 0;
}