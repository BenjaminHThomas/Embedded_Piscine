#include "eeprom.h"

unsigned char EEPROM_read(unsigned int uiAddress) {
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
}

void EEPROM_write(unsigned int uiAddress, unsigned char ucData) {
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	/* Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
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
	for (uint32_t i = 0; i <= 1008;) { // 1008 + 16 = 1024 == 1kb
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