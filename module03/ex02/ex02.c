// Ok, now even more colors! You must write a program that controls the RGB LED
// D5 but with PWM. Make the colors transition from one to the next smoothly using the
// provided wheel function.
// • Write a function init_rgb() that initializes the timers.
// • Then write a function set_rgb() that allows you to set the color of the LED.
// • Finally, your program must display the color wheel

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define RGB_R (1 << PD5)
#define RGB_G (1 << PD6)
#define RGB_B (1 << PD3)

#define ANIM_DELAY 10

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	//r
	OCR0B = 255 - r;
	//g
	OCR0A = 255 - g;
	//b
	OCR2B = 255 - b;
}

void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	} else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} else {
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

void init_rgb(void) {
	// OC0B red timer
	// OC0A green timer
	// OC2B blue timer

	// init red - timer 0
	TCCR0A |= (1 << COM0B1) | (1 << COM0B0); // Set OC0B on compare match, clear OC0B at BOTTOM, (inverting mode)
	TCCR0A |= (1 << WGM00); // PWM, phase correct, 255 MAX
	TCCR0B |= (1 << CS01); // 8 bit prescaler

	// // init green
	TCCR0A |= (1 << COM0A1) | (1 << COM0A0); // Set OC0A on compare match, clear OC0A at BOTTOM, (inverting mode)

	// // init blue
	TCCR2A |= (1 << COM2B1) | (1 << COM2B0); // set uptick, clear downtick
	TCCR2A |= (1 << WGM20); // PWM, phase correct, 255 max
	TCCR2B |= (1 << CS21); // 8 prescaler
}

int main(void) {
	uint8_t pos = 0;
	init_rgb();
	DDRD |= RGB_R | RGB_G | RGB_B;
	while (1) {
		wheel(pos++);
		_delay_ms(ANIM_DELAY);
	}
	return 0;
}
