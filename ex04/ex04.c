#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

#define LED_1 (1 << PB0)
#define LED_2 (1 << PB1)
#define LED_3 (1 << PB2)
#define LED_4 (1 << PB4) // need to change this not to refer to MISO
#define LED_4_OFF (0 << PB4)
#define BUTT_SW1 (0 << PD2)
#define BUTT_SW1_ON (1 << PD2)
#define BUTT_SW2 (0 << PD4)
#define BUTT_SW2_ON (1 << PD4)

#define CHECK_BIT(var, pos) ((var) & (1<<(pos - 1)))

int main(void) {
	int counter = 0;

	DDRD = BUTT_SW1 | BUTT_SW2;
	DDRB = LED_1 | LED_2 | LED_3 | LED_4;
	while (1) {
		if (!(PIND & BUTT_SW1_ON)) {
			++counter;
		}
		if (!(PIND & BUTT_SW2_ON)) {
			--counter;
		}
		PORTB = counter;
		PORTB = PORTB & ~(LED_4);
		if (CHECK_BIT(counter, 4)) {
			PORTB |= LED_4;
		}
		_delay_ms(275);
	}
	return 0;
}