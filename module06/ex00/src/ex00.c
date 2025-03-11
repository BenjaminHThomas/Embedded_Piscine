//• The AVR ATmega328P microcontroller has 1 I2C peripheral that you must use in
//this exercise to communicate with an AHT20 temperature sensor (U3).
//• You must write a function i2c_init that initializes the I2C on the microcontroller.
//• The MCU’s I2C must be configured so that the communication frequency is 100kHz.
//• You must write a function i2c_start that starts an I2C transmission between the
//microcontroller and the sensor.
//• The program must return status values to your computer after each data transmis-
//sion.
//• You must write a function i2c_stop that interrupts communication between the
//microcontroller and the sensor.

#include "uart.h"

#define AHT20_ADDRESS 0x38
#define AHT20_WRITE (AHT20_ADDRESS << 1) // set last bit to 0 = 0x70
#define AHT20_READ ((AHT20_ADDRESS << 1) | 1) // set last bit to 1 = 0x71

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void output_error(uint8_t status);

int main(void) {
	uart_init(MYUBRR);
	i2c_init();
	while (1) {
		i2c_start();
		i2c_stop();
	}
	return 0;
}

void i2c_init(void) {
	TWSR = 0x00; // no prescaler
	// 100000 = (16000000)/(16 + 2(TWBR) * 1)
	// TWBR = 72
	TWBR = 72; // 100kHz
	TWCR = (1 << TWEN); // enable TWI
}

void i2c_start(void) {
	volatile uint8_t status;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))); // wait for response
	status = TWSR & 0xF8; // get status from register
	output_error(status);
	TWDR = AHT20_WRITE; // send slave address & write bit
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	status = TWSR & 0xF8;
	output_error(status);
}

void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while(TWCR & (1 << TWSTO)); // wait for stop condition to be executed
}

void output_error(uint8_t status) {
	if (status == TW_START)
		uart_tx_str("START acknowledged.\r\n");
	else if (status == TW_MT_SLA_ACK)
		uart_tx_str("success\r\n");
	else if (status == TW_MT_SLA_NACK)
		uart_tx_str("error: address send, nack received\r\n");
	else if (status == TW_MT_DATA_NACK)
		uart_tx_str("error: data send, nack received\r\n");
	else
		uart_tx_str("error: other\r\n");
}