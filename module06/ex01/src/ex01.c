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

// SDA = Serial Data Line
// SCL = Serial Clock Line

#include "uart.h"
#include "aht20.h"

int main(void) {
	uart_init(MYUBRR);
	uart_tx_str("init i2c...\r\n");
	i2c_init();
	uart_tx_str("init aht20...\r\n");
	aht20_init();
	uart_tx_str("starting main loop...\r\n");
	aht20_start_measure();
	while (1) {
		i2c_start();
		i2c_write(AHT20_READ);
		// print_status();

		i2c_read(); // state
		// print_status();
		i2c_read(); // humidity0
		// print_status();
		i2c_read(); // humidity1
		// print_status();
		i2c_read(); // humidity2
		// print_status();
		i2c_read(); // temp0
		// print_status();
		i2c_read(); // temp1
		// print_status();
		i2c_read(); // crc
		// print_status();
		
		i2c_stop();
		uart_tx_str("\r\n");
		_delay_ms(100);
	}
	return 0;
}

void aht20_init(void) {
	_delay_ms(40);
	aht20_send_cmd(AHT20_INIT); // init
	_delay_ms(10);
}

void aht20_start_measure(void) {
	aht20_send_cmd(AHT20_TRIG_MES);
	_delay_ms(80);
}

// void ath20_send_full_cmd(uint8_t cmd, uint8_t param1, uint8_t param2) {
// 	i2c_start();
// 	i2c_write(AHT20_WRITE);
// 	i2c_write(cmd);
// 	i2c_write(param1);
// 	i2c_write(param2);
// 	i2c_stop();
// }

void aht20_send_cmd(uint8_t cmd) {
	i2c_start();
	i2c_write(AHT20_WRITE);
	i2c_write(cmd);
	i2c_stop();
}

void aht20_read_cmd() {
	i2c_start();
	i2c_write(AHT20_READ);
	i2c_read();
	i2c_stop();
}

void i2c_write(unsigned char data) {
	TWDR = data;  // copy data in TWI data register
	TWCR = (1<<TWINT) | (1 << TWEN); // enable TWI and clear interrupt flag
	while(!(TWCR & (1<<TWINT))); // wait for data to be sent
}

void i2c_read(void) {
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); 
	while (!(TWCR & (1 << TWINT)));
	volatile char result = TWDR;
	print_hex_value(result);
	uart_tx_str(" ");
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
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
	while (!(TWCR & (1<<TWINT)));
}

void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while(!(TWCR & (1 << TWSTO))); // wait for stop condition to be executed
}

void print_status(void)
{
	uint8_t status = TWSR & 0xF8;
	switch (status) {
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