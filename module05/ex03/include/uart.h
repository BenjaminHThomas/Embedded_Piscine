#ifndef UART_H
# define UART_H

# include <avr/io.h>
# define F_CPU 16000000
# include <util/delay.h>
# include <avr/interrupt.h>

# define BAUD 115200
# define MYUBRR F_CPU/16/(BAUD-1)

# define ENTERKEY 13
# define BACKSPACE 8
# define DELETE 127

void uart_init(unsigned int ubrr);
void uart_tx(char c);
char uart_rx(void);
void backspace(void);
void uart_tx_str(char *str);
void get_uinput(char * buff, unsigned int maxSize);

#endif