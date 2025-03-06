// Ok now, more colors! You will have to write a program that controls the RGB LED
// D5. Here is a table of colors that should be displayed successively, in a loop and changing
// color every second.

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

// #define BAUD 115200
// #define MYUBRR F_CPU/16/(BAUD-1)
#define PRESCALER 1024
#define COMPARE F_CPU/PRESCALER

#define RGB_R (1 << PD5)
#define RGB_G (1 << PD6)
#define RGB_B (1 << PD3)
#define RGB_Y (RGB_R | RGB_G)
#define RGB_C (RGB_G | RGB_B)
#define RGB_M (RGB_R | RGB_B)
#define RGB_W (RGB_R | RGB_G | RGB_B)

// typedef struct s_colours {
const uint8_t colour_list[] = {RGB_R, RGB_G, RGB_B, RGB_Y, RGB_C, RGB_M, RGB_W};
uint8_t curr_pos = 0;
const uint8_t max_size = 6;
// }	t_colours;

// t_colours colours;

ISR(TIMER1_OVF_vect) {
	if (curr_pos < max_size)
		++curr_pos;
	else
		curr_pos = 0;
	PORTD = colour_list[curr_pos];
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
