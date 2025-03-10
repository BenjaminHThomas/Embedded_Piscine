// Read the value of the linear potentiometer RV1 using the ADC peripheral.
// • ADC must be configured with an 8-bit resolution and AVCC as a reference.
// • Then display its value in hexadecimal format every 20ms on the console.

#include "uart.h"
#include "libft.h"
#include "stdio.h"

#define BUFFSIZE 5

char buff[BUFFSIZE];


// ISR(ADC_vect) {
// 	while (ADCSRA & (1 << ADSC));
// 	unsigned char pos = ADCH;
// 	ft_bzero(buff, BUFFSIZE);
// 	ft_itoa(pos, buff);
// 	uart_tx_str(buff, 0);
// 	_delay_ms(20);
// }


uint8_t adc_read(void) {
	ADCSRA |= (1 << ADSC); // start conversion
	while ((ADCSRA & (1 << ADSC))); // wait until conversion is finished
	return ADCH;
}


void init(void) {
	ADMUX |= (1 << REFS0); // avcc voltagee
	ADMUX |= (1 << ADLAR); // 8 bits

	// 0 MUX registers == slider

	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 bit prescaler
	ADCSRA |= (1 << ADEN); // enable adc module
	// ADCSRA |= (1 << ADIE); // enable interrupts
	// ADCSRA |= (1 << ADSC); // start a conversion
	// sei();
}

int main(void) {
	uart_init(8);
	uart_tx_str("Uart initialised\r\n");
	init();
	uart_tx_str("ADC initialised\r\n");
	while(1) {
		uint8_t pos = adc_read();
		sprintf(buff, "%02x", pos);
		uart_tx_str(buff);
		uart_tx_str("\r\n");
		_delay_ms(20);
	}
	return 0;
}