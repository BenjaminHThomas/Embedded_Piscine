// • Read the potentiometer RV1 + the LDR (R14) + the NTC (R20)
// • But this time, you must use the ADC in 10bit mode
// • After that, you must display the values in decimal format every 20ms on the console

#include "uart.h"
#include "libft.h"
#include "stdio.h"

#define RV1_PIN 0
#define LDR_PIN (1 << MUX0)
#define NTC_PIN (1 << MUX1)

// ISR(ADC_vect) {
// 	while (ADCSRA & (1 << ADSC));
// 	unsigned char pos = ADCH;
// 	ft_bzero(buff, BUFFSIZE);
// 	ft_itoa(pos, buff);
// 	uart_tx_str(buff, 0);
// 	_delay_ms(20);
// }

void print_results(uint16_t rv1, uint16_t ldr, uint16_t ntc) {
	char results[15];

	sprintf(results, "%u, %u, %u\r\n", rv1, ldr, ntc);
	uart_tx_str(results);
}

uint16_t adc_read(uint8_t pin) {
	ADMUX = (1 << REFS0);
	ADMUX |= pin;
	ADCSRA |= (1 << ADSC); // start conversion
	while ((ADCSRA & (1 << ADSC))); // wait until conversion is finished
	return ADC;
}


void init(void) {
	ADMUX |= (1 << REFS0); // avcc voltagee
	// ADMUX |= (1 << ADLAR); // 8 bits

	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 bit prescaler
	ADCSRA |= (1 << ADEN); // enable adc module
	// ADCSRA |= (1 << ADIE); // enable interrupts
	// ADCSRA |= (1 << ADSC); // start a conversion
	// sei();
}

int main(void) {
	// rv1 = slider
	// ldr = light
	// ntc = temp
	uint16_t rv1, ldr, ntc;
	uart_init(8);
	init();
	while(1) {
		rv1 = adc_read(RV1_PIN);
		ldr = adc_read(LDR_PIN);
		ntc = adc_read(NTC_PIN);
		print_results(rv1, ldr, ntc);
		_delay_ms(20);
	}
	return 0;
}