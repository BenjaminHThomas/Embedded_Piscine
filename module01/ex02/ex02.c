#include <avr/io.h>

#define LED_2 (1 << PB1)

// You are required to write a program that 
// turns on and off LED D2 (PB1) at a
// frequency of 1Hz with a duty cycle of 10%.

// PWM, count going up and down
// F_CPU = 16000000

#define F_CPU 16000000
#define PRESCALER 256
#define TOP (F_CPU / PRESCALER / 2) // 31250
#define COMPARE (TOP * 0.9) // 28125

int main(void) {
	DDRB |= LED_2;

	TCCR1A |= ((1 << COM1A1) | (1 << COM1A0)); // set when up-counting, clear when down-counting
	TCCR1A |= (1 << WGM11); // PWM phase correct
	TCCR1B |= (1 << WGM13); // PWM phase correct
	// ICR1 = 28125;
	OCR1A = COMPARE;
	ICR1 = TOP;
	TCCR1B |= (1 << CS12); // 256 prescaler

	while (1) {
		;
	}
}