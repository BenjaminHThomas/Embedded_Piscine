//• Complete the previous program by adding a function i2c_write which will write the
//contents of the TWDR register of the microcontroller and send it to the temperature
//sensor.
//• You must write a function i2c_read which will display the contents of the TWDR
//register after the sensor measurement.
//• You must write a function print_hex_value which will write the contents of the
//7 bytes of an AHT20 sensor measurement without modification to the standard
//output of your PC.
//• You must display the return values in a loop that repeats with a frequency that
//meets the manufacturer’s recommendations

#include "uart.h"

#define AHT20_ADDRESS 0x38
#define AHT20_WRITE (AHT20_ADDRESS << 1) // set last bit to 0 = 0x70
#define AHT20_READ ((AHT20_ADDRESS << 1) | 1) // set last bit to 1 = 0x71

#define HEX "0123456789ABCDEF"

// ex00
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void print_status(uint8_t status_code);

// ex01
void i2c_write(unsigned char data);
void i2c_read(void);
void print_hex_value(char c);

int main(void) {
	uart_init(MYUBRR);
	i2c_init();
	_delay_ms(40);
	while (1) {
		i2c_start();
		i2c_write(0xAC);
		_delay_ms(80);
		i2c_read();
		i2c_stop();
	}
	return 0;
}

void i2c_write(unsigned char data) {
	uint8_t status;
	TWDR = data; // copy data in TWI data register
	TWCR = (1 << TWEN) | (1 << TWINT); // enable TWI and clear interrupt flag
	while(!(TWCR & (1 << TWINT))); // wait on write
	status = TWSR & 0xF8;
	print_status(status);
}

void i2c_read(void) {
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA); // Enable TWI, generation of ack
	while(!(TWCR&(1<<TWINT))); // wait on read finish
	char result = TWDR;
	print_hex_value(result);
	uart_tx_str("\r\n");
}

void print_hex_value(char c) {
	unsigned char uc = (unsigned char)c;

	if (c < 0) {
		uart_tx('-');
		uc = (unsigned char)(-c);
	}
	if (uc < 16) {
		uart_tx(HEX[uc]);
		return ;
	}
	print_hex_value(uc / 16);
	uart_tx(HEX[uc % 16]);
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
	if (status != TW_START)
		print_status(status);
	TWDR = AHT20_WRITE; // send slave address & write bit
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	status = TWSR & 0xF8;
	if (status != TW_MT_SLA_ACK)
		print_status(status);
}

void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while(TWCR & (1 << TWSTO)); // wait for stop condition to be executed
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