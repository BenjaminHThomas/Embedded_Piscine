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
#include <stdio.h> // sprintf

uint8_t sensor_data[7];
float humid_readings[3];
float temp_readings[3];
uint8_t read_count = 0;

int main(void) {
	uart_init(MYUBRR);
	uart_tx_str("init i2c...\r\n");
	i2c_init();
	uart_tx_str("init aht20...\r\n");
	aht20_init();
	uart_tx_str("starting main loop...\r\n");
	while (1) {
		while (read_count < 3) {
			aht20_start_measure();
			read_sensor();
			calc_th();
			++read_count;
		}
		print_avg_readings();
		read_count = 0;
	}
	return 0;
}

void aht20_init(void) {
	_delay_ms(40);
	aht20_send_full_cmd(AHT20_INIT, 0x08, 0x00);
	_delay_ms(10);
}

void aht20_start_measure(void) {
	aht20_send_full_cmd(AHT20_TRIG_MES, 0x33, 0x00);
	_delay_ms(80);
}

void read_sensor(void) {
	i2c_start();
	i2c_write(AHT20_READ);
	for (uint8_t i = 0; i < 6; i++) {
		sensor_data[i] = i2c_read();
	}
	sensor_data[6] = read_last_byte(); // crc
	i2c_stop();
}

void calc_th(void) {
	int32_t hum;
	hum = ( (int32_t)sensor_data[1] << 12 ) | ((int32_t)sensor_data[2] << 4) | ( (sensor_data[3] & 0b11110000) >> 4 );
	humid_readings[read_count]  = ((float)hum/1048576.0)*100.0;

	int32_t temp;
	temp = ( ((int32_t)sensor_data[3] & 0b00001111) << 16 ) | ((int32_t)sensor_data[4] << 8) | (sensor_data[5]);
	temp_readings[read_count] = ((float)temp/1048576.0)*200.0-50.0;
}


void print_avg_readings(void) {
	float avg_humid = (humid_readings[0] + humid_readings[1] + humid_readings[2])/3.0;
	float avg_temp = (temp_readings[0] + temp_readings[1] + temp_readings[2])/3.0;
	char buff[40];
	sprintf(buff,"Temperature: %.2fC, Humidity: %.3f%%\r\n", avg_temp, avg_humid);
	uart_tx_str(buff);
}


void aht20_send_full_cmd(uint8_t cmd, uint8_t param1, uint8_t param2) {
 	i2c_start();
 	i2c_write(AHT20_WRITE);
 	i2c_write(cmd);
 	i2c_write(param1);
 	i2c_write(param2);
 	i2c_stop();
 }

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

uint8_t i2c_read(void) {
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // send ack
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

uint8_t read_last_byte(void) {
	TWCR = (1 << TWINT) | (0 << TWEA) | (1 << TWEN); // send nack
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

void print_hex_value(char c) {
	unsigned char uc = (unsigned char)c;

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