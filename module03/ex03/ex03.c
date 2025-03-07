// You must make a program that listen to the serial port.
// When you send a new line with an HEX RGB color with the format #RRGGBB sets the
// color on the RGB LED on pin D5.

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR F_CPU/16/(BAUD-1)

#define RGB_R (1 << PD5)
#define RGB_G (1 << PD6)
#define RGB_B (1 << PD3)

#define ENTERKEY 13
#define BACKSPACE 8
#define DELETE 127

#define HEX "0123456789ABCDEF"
#define BUFFSIZE 6

void uart_init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// set frame format: 8data
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	// UCSR0B |= (1 << RXCIE0); // enable rx interrupt
}

void uart_tx(char c) {
	while (!(UCSR0A & (1<<UDRE0))) ;
	UDR0 = c;
}

char uart_rx(void) {
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void backspace(void) {
	uart_tx(BACKSPACE);
	uart_tx(' ');
	uart_tx(BACKSPACE); 
}

uint8_t get_hex_val(char c)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        if (c == HEX[i])
            return (i);
    }
	return 0;
}

uint8_t get_c_val(char c1, char c2) {
	return (get_hex_val(c1) * 16) + get_hex_val(c2);
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

unsigned char is_valid_char(char c) {
	if ((c >= '0' && c <= '9') ||
		(c >= 'A' && c <= 'F')) {
			return 1;
	}
	return 0;
}

void get_uinput(char * buff) {
	char c = 0;
	for (uint8_t i = 0; i < BUFFSIZE; i++){
		buff[i] = 0;
	}
	uint8_t i = 0;
	uart_tx('#');
	while (c != ENTERKEY) {
		c = uart_rx();
		if (i < BUFFSIZE && is_valid_char(c)) {
			buff[i++] = c;
			uart_tx(c);
		}
		if ((c == BACKSPACE || c == DELETE ) && i > 0) {
			backspace();
			buff[--i] = 0;
		}
	}
	uart_tx('\r');
	uart_tx('\n');
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	//r
	OCR0B = 255 - r;
	//g
	OCR0A = 255 - g;
	//b
	OCR2B = 255 - b;
}

void get_rgb(uint8_t * r, uint8_t * g, uint8_t * b, char * input) {
	*r = get_c_val(input[0], input[1]);
	*g = get_c_val(input[2], input[3]);
	*b = get_c_val(input[4], input[5]);
}

int main(void) {
	char input[6];
	uint8_t r, g, b;

	DDRD |= RGB_R | RGB_G | RGB_B;
	PORTD = 0;
	uart_init(MYUBRR);
	init_rgb();
	set_rgb(16, 16, 16);
	while(1) {
		get_uinput(input);
		get_rgb(&r, &g, &b, input);
		set_rgb(r, g , b);
	}
}