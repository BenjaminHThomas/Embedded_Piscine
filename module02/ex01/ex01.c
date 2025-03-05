// • You must write a function uart_printstr that will be called every 2 seconds to
// display Hello World! on the serial port.
// • The MCU’s UART must be configured to 115200 8N1.
// • The infinite loop of the program should remain empty.

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR F_CPU/16/(BAUD-1)
#define PRESCALER 1024
#define COMPARE F_CPU/PRESCALER

void uart_init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// set frame format: 8data
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}

void uart_tx(char *str) {
	if (!str)
		return ;
	while (*str) {
		while (!(UCSR0A & (1<<UDRE0))) ;
		UDR0 = *str++;
	}
}

ISR(TIMER1_OVF_vect) {
	uart_tx("Hello World!\n\r");
	TCNT1 = 65535 - COMPARE;
}

int main(void) {
	uart_init(MYUBRR);
	TCNT1 = 65535 - COMPARE; // max 16bit - 1hz with 1024 prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10); // 1024 prescaler
	TCCR1A = 0x00; // disable flags - no compare
	TIMSK1 = (1 << TOIE1); // enable overflow interrupt

	sei(); //set enabled interrupts
	while (1){
	}
}