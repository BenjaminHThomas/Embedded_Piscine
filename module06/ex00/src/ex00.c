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

void print_status(uint8_t status_code);

int main(void) {
	volatile uint8_t status;

	uart_init(MYUBRR);
	i2c_init();
	while (1) {
		i2c_start();
		TWDR = AHT20_WRITE; // send slave address & write bit
		TWCR = (1 << TWINT) | (1 << TWEN); // turn on twi
		while (!(TWCR & (1 << TWINT))); // wait until data is sent to control register
		status = TWSR & 0xF8; // get status from register
		print_status(status);
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
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
	while (!(TWCR & (1<<TWINT)));
}

void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while(!(TWCR & (1 << TWSTO))); // wait for stop condition to be executed
}

void print_status(uint8_t status_code)
{
	switch (status_code) {
		case TW_START:
			uart_tx_str ("START acknowledge.\r\n");
			break;
		case TW_REP_START:
			uart_tx_str ("REPEATED START acknowledge.\r\n");
			break;
		case TW_MT_SLA_ACK:
			uart_tx_str ("Master Transmitter: Slave ACK\r\n");
			break;
		case TW_MT_SLA_NACK:
			uart_tx_str ("Master Transmitter : Slave NACK\r\n");
			break;
		case TW_MT_DATA_ACK:
			uart_tx_str ("Master Transmitter : Data ACK\r\n");
			break;
		case TW_MT_DATA_NACK:
			uart_tx_str ("Master Transmitter: Data NACK\r\n");
			break;
		case TW_MR_SLA_ACK:
			uart_tx_str ("Master Receiver : Slave ACK\r\n");
			break;
		case TW_MR_SLA_NACK:
			uart_tx_str ("Master Receiver : Slave NACK\r\n");
			break;
		case TW_MR_DATA_ACK:
			uart_tx_str ("Master Receiver : Data ACK\r\n");
			break;
		case TW_MR_DATA_NACK:
			uart_tx_str ("Master Receiver : Data NACK\r\n");
			break;
		case TW_MT_ARB_LOST:
			uart_tx_str ("Arbitration Lost\r\n");
			break;
		case TW_ST_SLA_ACK:
			uart_tx_str ("Slave Transmitter : Slave ACK\r\n");
			break;
		case TW_ST_ARB_LOST_SLA_ACK:
			uart_tx_str ("Arbitration Lost in SLA+R/W, Slave ACK\r\n");
			break;
		case TW_ST_DATA_ACK:
			uart_tx_str ("Slave Transmitter : Data ACK\r\n");
			break;
		case TW_ST_DATA_NACK:
			uart_tx_str ("Slave Transmitter : Data NACK\r\n");
			break;
		case TW_ST_LAST_DATA:
			uart_tx_str ("Slave Transmitter : Last Data\r\n");
			break;
		case TW_SR_SLA_ACK:
			uart_tx_str ("Slave Receiver : Slave ACK\r\n");
			break;
		case TW_SR_ARB_LOST_SLA_ACK:
			uart_tx_str ("Arbitration Lost in SLA+R/W, Slave ACK\r\n");
			break;
		case TW_SR_GCALL_ACK:
			uart_tx_str ("General Call : Slave ACK\r\n");
			break;
		case TW_SR_ARB_LOST_GCALL_ACK:
			uart_tx_str ("Arbitration Lost in General Call, Slave ACK\r\n");
			break;
			case TW_SR_DATA_ACK:
			uart_tx_str ("Slave Receiver : Data ACK\r\n");
			break;
		case TW_SR_DATA_NACK:
			uart_tx_str ("Slave Receiver : Data NACK\r\n");
			break;
		case TW_SR_GCALL_DATA_ACK:
			uart_tx_str ("General Call : Data ACK\r\n");
			break;
		case TW_SR_GCALL_DATA_NACK:
			uart_tx_str ("General Call : Data NACK\r\n");
			break;
		case TW_SR_STOP:
			uart_tx_str ("Slave Receiver : STOP received\r\n");
			break;
		case TW_NO_INFO:
			uart_tx_str ("No state information available\r\n");
			break;
		case TW_BUS_ERROR:
			uart_tx_str ("Bus Error\r\n");
			break;
		default:
			uart_tx_str ("Unknown Status Code\r\n");
	}
}