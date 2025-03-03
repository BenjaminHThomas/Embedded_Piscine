#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

#define LED_1 (1 << PB0)
#define BUTT_SW1 (0 << PD2)
#define BUTT_SW1_ON (1 << PD2)

int main(void) {
	DDRB |= LED_1;
	DDRD |= BUTT_SW1;

	while (1) {
		if (!(PIND & BUTT_SW1_ON)) {
			PORTB ^= LED_1;
			_delay_ms(300);
		}
	}
	return 0;
}