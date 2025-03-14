#include "utils.h"

void print_hex_value(uint16_t uc) {
	if (uc < 16) {
		uart_tx(HEX[uc]);
		return ;
	}
	print_hex_value(uc / 16);
	uart_tx(HEX[uc % 16]);
}

uint8_t is_in(const char * buff, unsigned char c) {
	while (*buff++) {
		if (*buff == c)
			return 1;
	}
	return 0;
}

uint8_t get_hexval(unsigned char c) {
	for (uint8_t i = 0; i < 16; i++) {
		if (HEX[i] == c)
			return i;
	}
	return 0;
}

// 256 == 598

uint16_t hex_to_dec(char * hexchar, uint8_t maxlen) {
	uint16_t val = 0;
	uint8_t hexval;
	for (uint8_t i = 0; i < maxlen; i++) {
		hexval = get_hexval(hexchar[i]);
		val *= 16;
		val += hexval;
	}
	return val;
}