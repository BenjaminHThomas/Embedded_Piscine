#include <avr/io.h>

# define LED_1 (1 << PB0)

int main(void) {
	// DDRX = Data Direction X
	// DDRB = Data Direction B
	// below I set all bits in the DDRB to 1 (output)
	
	// DDRB = 0b1111111;
	DDRB |= LED_1;

	// PORTX sets the output state of the pins
	// PORTB = 1 sets the output state to HIGH (+5V)
	PORTB |= LED_1;
	return 0;
}