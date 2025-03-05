// • Create 2 strings, a username and a password.
// • Display a prompt on the serial port that asks for the username and password.
// • When typing the username, there should be an echo.
// • same for the password but with ’*’ instead of characters.
// • The Backspace key deletes a character.
// • The Enter key validates the input.
// • If the username and password are correct, the program displays the welcome text
// and makes the LEDs blink.
// • Otherwise, the program displays the error message.

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR F_CPU/16/(BAUD-1)
#define PRESCALER 1024
#define COMPARE F_CPU/PRESCALER/4
#define ENTERKEY 13
#define BACKSPACE 8
#define DELETE 127
#define USERNAME "admin"
#define PASSWORD "pass"
#define LED_1 (1 << PB0)
#define LED_2 (1 << PB1)
#define LED_3 (1 << PB2)
#define LED_4 (1 << PB4)

// colours
#define BLU "\e[0;34m"
#define GRN "\e[0;32m"
#define BRED "\e[1;31m"
#define CRESET "\e[0m"


#define CHECK_BIT(var, pos) ((var) & (1<<(pos - 1)))

uint8_t ledCounter = 0;

int ft_strcmp(char *s1, char *s2) {
	if (!s1 && !s2)
		return 0;
	else if (!s1 || !s2)
		return -1;
	char *ps1 = &s1[0];
	char *ps2 = &s2[0];

	while (*ps1 && *ps2 && *ps1 == *ps2) {
		++ps1;
		++ps2;
	}
	return *ps1 - *ps2;
}

void uart_tx(char c) {
	while (!(UCSR0A & (1<<UDRE0))) ;
	UDR0 = c;
}

void uart_tx_str(char *str, char *colour) {
	if (!str || !colour)
		return ;
	while (*colour) {
		uart_tx(*colour++);
	}
	while (*str) {
		uart_tx(*str++);
	}
}

char uart_rx(void) {
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void uart_init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// set frame format: 8data
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	// UCSR0B |= (1 << RXCIE0); // enable rx interrupt
}

void led_init(void) {
	DDRB = LED_1 | LED_2 | LED_3 | LED_4;

	// 1 hz timer
	TCNT1 = 65535 - COMPARE; // max 16bit - 1hz with 1024 prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10); // 1024 prescaler
	TCCR1A = 0x00; // disable flags - no compare
	TIMSK1 = (1 << TOIE1); // enable overflow interrupt
}

ISR(TIMER1_OVF_vect) {
	++ledCounter;
	PORTB = ledCounter;
	PORTB = PORTB & ~(LED_4);
	if (CHECK_BIT(ledCounter, 4)) {
		PORTB |= LED_4;
	}
	TCNT1 = 65535 - COMPARE;
}

void backspace(void) {
	uart_tx(BACKSPACE);
	uart_tx(' ');
	uart_tx(BACKSPACE); 
}

void get_uinput(char * buff, int isPass) {
	int i = 0;
	char c = 0;
	for (uint8_t i = 0; i < 20; i++){
		buff[i] = 0;
	}
	uart_tx_str(" ", CRESET);
	while (c != ENTERKEY) {
		c = uart_rx();
		if (i < 20 && c != ENTERKEY && c != BACKSPACE && c != DELETE) {
			buff[i++] = c;
			if (!isPass)
				uart_tx(c);
			else
				uart_tx('*');
		}
		if ((c == BACKSPACE || c == DELETE ) && i > 0) {
			backspace();
			buff[--i] = 0;
		}
	}
	uart_tx_str("\r\n", CRESET);
}

int main(void) {
	char username[20];
	char password[20];
	uart_init(MYUBRR);
	led_init();
	sei();
	while (1){
		uart_tx_str("Username:", BLU);
		get_uinput(username, 0);
		uart_tx_str("Password:", BLU);
		get_uinput(password, 1);
		if (ft_strcmp(username, USERNAME) == 0 && ft_strcmp(password, PASSWORD) == 0) {
			uart_tx_str("Welcome m8, come on in\r\n", GRN);
			uart_tx_str("You are now trapped here\n\r\n", BRED);
			break ;
		} else {
			uart_tx_str("Incorrect credentials, try again.\r\n", BRED);
		}
	}
	while (1) {
		;;
	}
}
