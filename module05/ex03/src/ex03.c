// The Atmega328P is able to read its internal temperature. It’s not ultra precise but
// we will read it anyway.
// • Read the internal temperature sensor value
// • Then display it on the console and convert it to celcius every 20ms

#include "uart.h"
#include "libft.h"
#include "stdio.h"

#define RTC_PIN (1 << MUX3)
// #define TS_GAIN 0x0003
// #define TS_OFFSET 0x0002
#define TEMP_OFFSET 334

void print_results(uint16_t temp_c) {
	char results[5];

	sprintf(results, "%u\r\n", temp_c);
	uart_tx_str(results);
}

void init(void) {
	ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 bit prescaler
	ADCSRA |= (1 << ADEN); // enable adc module.
	ADMUX = (1 << REFS0) | (1 << REFS1); // internal 1.1V
	ADMUX |= RTC_PIN;
}

uint16_t get_temp(void) {
	ADCSRA |= (1 << ADSC); // start conversion
	while ((ADCSRA & (1 << ADSC))); // wait until conversion is finished
	// return (((((ADCH << 8) + ADCL) - (273 + 100 - TS_OFFSET)) * 128) / TS_GAIN) + 25;
	// return ((ADCL | (ADCH << 8))) - TEMP_OFFSET;
	return ADC - TEMP_OFFSET;
}

int main(void) {
	uart_init(MYUBRR);
	init();
	while(1) {
		print_results(get_temp());
		_delay_ms(20);
	}
	return 0;
}