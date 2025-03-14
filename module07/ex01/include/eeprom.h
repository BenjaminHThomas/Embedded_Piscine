#ifndef EEPROM_H
# define EEPROM_H

# include <avr/eeprom.h>
# include "utils.h"


void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);
void print_address(uint16_t pos);
void print_eeprom(void);

#endif