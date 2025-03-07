// • You must configure Timer0 to trigger a periodic interrupt that varies the duty cycle
// of the LED PB1 controlled by Timer1.
// • The frequency of Timer1 must be high enough to no longer see the LED blinking.
// • Do not hesitate to use multiple Timer registers to complete this exercise.
// • The duty cycle should vary in a loop from 0% to 100% and then from 100% to 0%
// in 1 second

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define LED_2 (1 << PB1)

uint8_t step = 1;

ISR(TIMER0_OVF_vect) {
	if (OCR1A == 255 || OCR1A == 0)
		step *= -1;
	OCR1A += step;
	TCNT0 = 128;
}

void init(void) {
	// SW1
	// EICRA |= (1 << ISC01) | (1 << ISC00); // button realise triggers interrupt
	// EIMSK |= (1 << INT0); // enable INT0 interrupt

	// timer 0
	TCCR0B |= (1 << CS02); // 256 prescalar
	TCCR0A = 0x00; // no compares
	TIMSK0 |= (1 << TOIE0); // enable overflow interrupt
	TCNT0 = 128;

	// timer 1 - LED 2
	TCCR1A |= (1 << COM1A1) | (1 << COM1A0); // set uptick, clear downtick
	TCCR1A |= (1 << WGM10); // 8 bit PWM
	TCCR1B |= (1 << CS10); // no prescaler
	OCR1A = 254;

	sei();
	DDRB |= LED_2;
}

int main(void) {
	init();
	while(1);
	return 0;
}