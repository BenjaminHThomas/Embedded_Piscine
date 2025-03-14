#ifndef UTILS_H
# define UTILS_H

# include <avr/io.h>
# include "uart.h"

# define HEX "0123456789ABCDEF"

void print_hex_value(uint16_t uc);
uint8_t is_in(const char * buff, unsigned char c);
uint16_t hex_to_dec(char * hexchar, uint8_t maxlen);

#endif