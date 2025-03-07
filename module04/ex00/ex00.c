// • You are required to write a program that changes the state of LED D1 when the
// button SW1 is pressed.
// • You must use interrupts to read the state of the button. Reading the PINx registers
// is not allowed

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define LED_1 (1 << PB0)

ISR(INT0_vect) {
	PORTB ^= LED_1;
	_delay_ms(200);
	EIFR |= (1 << INTF0);
}

void init(void) {
	EICRA |= (1 << ISC01) | (1 << ISC00); // button realise triggers interrupt
	EIMSK |= (1 << INT0); // enable INT0 interrupt

	sei();
	DDRB |= LED_1;
}

int main(void) {
	init();
	while(1){
		;;
	}
}