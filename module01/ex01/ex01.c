#include <avr/io.h>
#define F_CPU 16000000

#define LED_2 (1 << PB1)

int main(void) {
	DDRB |= LED_2; // set as output
	TCCR1B |= ((1 << CS10) | (1 << CS12)); // set up timer with prescalar 1024

	// 16000000 / 1024 = 15625 ops per second
	// 15625 / 2 = 7812 oscilate per hz
	while (1) {
		if (TCNT1 >= 7812) {
			PORTB ^= LED_2;
			TCNT1 = 0;
		}
	}
	return 0;
}