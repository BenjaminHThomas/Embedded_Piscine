#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

#define LED_1 (1 << PB0)
#define LED_2 (1 << PB1)
#define LED_3 (1 << PB2)
#define LED_4 (1 << PB4) // need to change this not to refer to MISO
#define BUTT_SW1 (0 << PD2)
#define BUTT_SW1_ON (1 << PD2)
#define BUTT_SW2 (0 << PD4)
#define BUTT_SW2_ON (1 << PD4)

int main(void) {
	int counter = 0;

	DDRD = BUTT_SW1 | BUTT_SW2;
	DDRB = LED_1 | LED_2 | LED_3 | LED_4;
	while (1) {
		if (!(PIND & BUTT_SW1_ON)) {
			++counter;
			counter = counter % 4;
			if (counter == 1) {
				PORTB = LED_1;
			} else if (counter == 2) {
				PORTB = LED_2;
			} else if (counter == 3) {
				PORTB = LED_3;
			} else {
				PORTB = LED_4;
			}
			// PORTB = LED_1 ^ LED_2 ^ LED_3 ^ LED_4; // turns on all
			_delay_ms(300);
		}
	}
	return 0;
}