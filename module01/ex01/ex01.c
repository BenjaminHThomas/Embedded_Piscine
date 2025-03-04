#include <avr/io.h>
#define F_CPU 16000000

#define LED_2 (1 << PB1)
#define T1_PRESCALER 1024


// int main(void) {
// 	DDRB |= LED_2; // set as output
// 	TCCR1B |= ((1 << CS10) | (1 << CS12)); // set up timer with prescalar 1024

// 	// 16000000 / 1024 = 15625 ops per second
// 	// 15625 / 2 = 7812 oscilate per hz
// 	while (1) {
// 		if (TCNT1 >= 7812) {
// 			PORTB ^= LED_2;
// 			TCNT1 = 0;
// 		}
// 	}
// 	return 0;
// }



// 16000000 / 1024 = 15625 ops per second
// 15625 / 2 = 7812 oscilate per hz
int main(void) {
	DDRB |= LED_2;

	TCCR1B |= (1 << WGM12); // timer 1 CTC mode
	TCCR1A |= (1 << COM1A0); // enable timer 1 compare output channel A in toggle mode
	OCR1A = 7812; // CTC compares value to 0.5*1hz
	TCCR1B |= ((1 << CS10) | (1 << CS12)); // 1024 prescaler

	while (1){
		;;
	}
}