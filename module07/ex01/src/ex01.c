// Write a program that displays the status of the entire EEPROM on the UART in
// hexdump format.

// This exercise in unfinished

#include "uart.h"
#include "eeprom.h"
#include "utils.h"

void get_uinputs(void);
uint8_t validate_inputs(void);

char user_addr[4];
char user_byte[4];

int main(void) {
	uart_init(MYUBRR);
	while(1) {
		get_uinputs();
		if (validate_inputs()) {
			uart_tx_str()
		}
		print_eeprom();
	}
	return 0;
}

void get_uinputs(void) {
	uart_tx_str("address: ");
	get_uinput(user_addr, 3);
	uart_tx_str("byte value: ");
	get_uinput(user_byte, 3);
}

uint8_t validate_inputs(void) {
	for (uint8_t i = 0; i < 4; i++) {
		if (!is_in(HEX, user_addr[i]))
			return 1;
		if (!is_in(HEX, user_byte[i]))
			return 1;
	}
	return 0;
}
