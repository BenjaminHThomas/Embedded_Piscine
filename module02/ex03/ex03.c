// • You must write a program that sends an echo on the serial port, but transforms
// lowercase characters into uppercase and uppercase characters into lowercase before
// sending them back.
// • Attention, this time instead of using your uart_rx, you must use an interruption
// to detect that a new character is on the UART port.
// • The infinite loop of the program must remain empty.

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

void uart_init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// set frame format: 8data
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}

char change_caps(char c) {
	if (c >= 'a' && c <= 'z')
		return c - 32;
	else if (c >= 'A' && c <= 'Z')
		return c + 32;
	return c;
}

ISR(USART_RX_vect) {
	char c = UDR0;
	if (c == ENTERKEY) {
		uart_tx('\r');
		uart_tx('\n');
	} else {
		uart_tx(change_caps(c));
	}
}

int main(void) {
	uart_init(MYUBRR);
	UCSR0B |= (1 << RXCIE0); // enable rx interrupt
	TCCR1A = 0x00; // disable flags - no compare
	sei(); //set enabled interrupts
	while (1){
		;;
	}
}