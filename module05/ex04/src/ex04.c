// Read the value of the linear potentiometer RV1 using the ADC peripheral.
// • ADC must be configured with an 8-bit resolution and AVCC as a reference.
// • Then display its value in hexadecimal format every 20ms on the console.

#include "uart.h"
#include "libft.h"
#include "stdio.h"

#define LED_1 (1 << PB0)
#define LED_2 (1 << PB1)
#define LED_3 (1 << PB2)
#define LED_4 (1 << PB4)
#define RGB_R (1 << PD5)
#define RGB_G (1 << PD6)
#define RGB_B (1 << PD3)

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	//r
	OCR0B = r / 3;
	//g
	OCR0A = g / 3;
	//b
	OCR2B = b / 3;
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

void update_leds(uint8_t pos) {
	PORTB = 0;
	if (pos >= 63)
		PORTB |= LED_1;
	if (pos >= 126)
		PORTB |= LED_2;
	if (pos >= 189)
		PORTB |= LED_3;
	if (pos >= 254)
		PORTB |= LED_4;
}

uint8_t adc_read(void) {
	ADCSRA |= (1 << ADSC); // start conversion
	while ((ADCSRA & (1 << ADSC))); // wait until conversion is finished
	return ADCH;
}

void init(void) {
	// sensor readings
	ADMUX |= (1 << REFS0); // avcc voltagee
	ADMUX |= (1 << ADLAR); // 8 bits

	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 bit prescaler
	ADCSRA |= (1 << ADEN); // enable adc module

	// LEDS 1 - 4
	DDRB = LED_1 | LED_2 | LED_3 | LED_4;

	// RGB
	// init red - timer 0
	TCCR0A |= (1 << COM0B1); // clear OC0B on compare match, set OC0B at BOTTOM, (inverting mode)
	TCCR0A |= (1 << WGM00); // PWM, phase correct, 255 MAX
	TCCR0B |= (1 << CS01); // 8 bit prescaler

	// // init green
	TCCR0A |= (1 << COM0A1); // clear OC0A on compare match, set OC0A at BOTTOM, (inverting mode)

	// // init blue
	TCCR2A |= (1 << COM2B1); // clear uptick, set downtick
	TCCR2A |= (1 << WGM20); // PWM, phase correct, 255 max
	TCCR2B |= (1 << CS21); // 8 prescaler
	DDRD |= RGB_R | RGB_G | RGB_B;
}

int main(void) {
	init();
	while(1) {
		uint8_t pos = adc_read();
		update_leds(pos);
		wheel(pos);
		_delay_ms(20);
	}
	return 0;
}