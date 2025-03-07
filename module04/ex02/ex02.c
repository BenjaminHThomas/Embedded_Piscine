// You must write a program that:
// ◦ each time you press the SW1 button increments a value
// ◦ each time you press the SW2 button decrements a value
// ◦ and displays this value permanently on LEDs D1 D2 D3 and D4 in binary.
// • You must use interrupts and have nothing in your main loop.

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_1 (1 << PB0)
#define LED_2 (1 << PB1)
#define LED_3 (1 << PB2)
#define LED_4 (1 << PB4)

#define CHECK_BIT(var, pos) ((var) & (1<<(pos - 1)))

volatile uint8_t num = 0;

void update_leds(int inc) {
	num += inc;
	PORTB = num;
	PORTB = num & ~(LED_4);
	if (CHECK_BIT(num, 4)) {
		PORTB |= LED_4;
	}
}

ISR(INT0_vect) {
	update_leds(1);
	_delay_ms(200);
	EIFR |= (1 << INTF0);
}

ISR(INT1_vect) {
	update_leds(-1);
	_delay_ms(200);
	EIFR |= (1 << INTF1);
}

void init(void) {
	EICRA |= (1 << ISC01) | (1 << ISC00); // button 1 realise triggers interrupt
	EICRA |= (1 << ISC11) | (1 << ISC10); // button 2 realise triggers interrupt
	
	EIMSK |= (1 << INT0); // enable INT0 interrupt
	EIMSK |= (1 << INT1); // enable INT1 interrupt

	sei();
	DDRB |= LED_1 | LED_2 | LED_3 | LED_4;
}

int main(void) {
	init();
	while(1);
	return 0;
}