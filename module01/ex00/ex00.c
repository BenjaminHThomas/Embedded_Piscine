#include <avr/io.h>

#define LED_2 (1 << PB1)

// Don't do this, it's just for an exercise
void delay500(void) {
	uint64_t count;

	count = 0;
	while (count < 100000UL)
		++count;
}

int main(void) {
	DDRB |= LED_2; // set as output

	while (1) {
		delay500();
		PORTB ^= LED_2;
	}
	return 0;
}