// Write a program that displays the status of the entire EEPROM on the UART in
// hexdump format.

#include "uart.h"
#include "eeprom.h"

void print_hex_value(uint16_t uc);
void print_address(uint16_t pos);
void print_eeprom(void);

int main(void) {
	uart_init(MYUBRR);
	print_eeprom();
	while(1);
	return 0;
}

void print_hex_value(uint16_t uc) {
	if (uc < 16) {
		uart_tx(HEX[uc]);
		return ;
	}
	print_hex_value(uc / 16);
	uart_tx(HEX[uc % 16]);
}

void print_address(uint16_t pos) {
	// 0000000
	uart_tx_str("0000");
	if (pos < 256) {
		uart_tx('0');
	}
	if (pos < 16) {
		uart_tx('0');
	}
	print_hex_value(pos);
	uart_tx(' ');
}

void print_eeprom(void) {
	unsigned char mem;
	for (uint32_t i = 0; i <= 1024;) {
		print_address(i);
		for (uint8_t byte = 0; byte < 16; byte += 2) {
			for (uint8_t x = 0; x < 2; x++) {
				mem = EEPROM_read(i++);
				print_hex_value(mem);
			}
			uart_tx_str(" ");
		}
		uart_tx_str("\r\n");
	}
}