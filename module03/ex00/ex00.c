// You must write a program that controls the RGB LED D5.
// • The LED should turn on in red, then green, then blue in a loop.
// • Changing every 1 second.

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR F_CPU/16/(BAUD-1)
#define PRESCALER 1024
#define COMPARE F_CPU/PRESCALER

#define RGB_R (1 << PD5)
#define RGB_G (1 << PD6)
#define RGB_B (1 << PD3)

uint8_t rgb_pos = 0;

ISR(TIMER1_OVF_vect) {
	// PORTD ^= RGB_R;
	if (rgb_pos < 2)
		++rgb_pos;
	else
		rgb_pos = 0;
	if (rgb_pos == 0)
		PORTD = RGB_R;
	else if (rgb_pos == 1)
		PORTD = RGB_G;
	else
		PORTD = RGB_B;
	TCNT1 = 65535 - COMPARE;
}

void timer_init(void) {
	TCNT1 = 65535 - COMPARE; // max 16bit - 1hz with 1024 prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10); // 1024 prescaler
	TCCR1A = 0x00; // disable flags - no compare
	TIMSK1 = (1 << TOIE1); // enable overflow interrupt
}

int main(void) {
	timer_init();
	DDRD |= RGB_R | RGB_G | RGB_B;
	sei();
	while (1) {
		;;
	}
	return 0;
}