#ifndef EEPROM_H
# define EEPROM_H

# include <avr/eeprom.h>
# define HEX "0123456789ABCDEF"

void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);

#endif