#ifndef AHT20_H
# define AHT20_H

// ex00
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void print_status(void);

// ex01
void i2c_write(unsigned char data);
uint8_t i2c_read(void);
void print_hex_value(char c);

// void get_ath_temp(void);
void aht20_init(void);
void aht20_send_full_cmd(uint8_t cmd, uint8_t param1, uint8_t param2);
void aht20_send_cmd(uint8_t cmd);
void aht20_read_cmd();
void aht20_start_measure(void);
uint8_t read_last_byte(void);
void read_sensor(void);

//ex02
void calc_th(void); // calc temp and humidity
void print_avg_readings(void);

# define AHT20_ADDRESS 0x38
# define AHT20_WRITE (AHT20_ADDRESS << 1) // set last bit to 0 = 0x70
# define AHT20_READ ((AHT20_ADDRESS << 1) | 1) // set last bit to 1 = 0x71
# define AHT20_INIT 0xBE //10111110
# define AHT20_TRIG_MES 0xAC // 10101100
# define AHT20_SOFT_RESET 0xBA // 10111010

# define HEX "0123456789ABCDEF"

#endif