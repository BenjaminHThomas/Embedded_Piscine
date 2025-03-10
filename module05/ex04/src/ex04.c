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


void update_leds(uint8_t pos) {
	PORTB = 0;
	if (pos >= 51)
		PORTB |= LED_1;
	if (pos >= 102)
		PORTB |= LED_2;
	if (pos >= 153)
		PORTB |= LED_3;
	if (pos >= 204)
		PORTB |= LED_4;
}

uint8_t adc_read(void) {
	ADCSRA |= (1 << ADSC); // start conversion
	while ((ADCSRA & (1 << ADSC))); // wait until conversion is finished
	return ADCH;
}

void init(void) {
	ADMUX |= (1 << REFS0); // avcc voltagee
	ADMUX |= (1 << ADLAR); // 8 bits

	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 bit prescaler
	ADCSRA |= (1 << ADEN); // enable adc module

	DDRB = LED_1 | LED_2 | LED_3 | LED_4;
}

int main(void) {
	init();
	while(1) {
		uint8_t pos = adc_read();
		update_leds(pos);
		_delay_ms(20);
	}
	return 0;
}