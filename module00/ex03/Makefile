# Board specs

MCU		= atmega168p
F_CPU	= 16000000
BAUD	= 115200

# Compile args

BIN		= ex03
CC		= avr-gcc
OBJCOPY	= avr-objcopy
INSTALL	= avrdude
FLAGS	= -Wall -Werror -Wextra -O1
FLAGS	+= -F_CPU=$(F_CPU) -mmcu=$(MCU)
PORT	= /dev/ttyUSB0

# Compile instructions

hex:
	$(CC) $(FLAGS) $(BIN).c -o $(BIN).bin
	$(OBJCOPY) -O ihex -R .eeprom $(BIN).bin $(BIN).hex

flash:
	$(INSTALL) -F -V -c arduino -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$(BIN).hex

all: hex flash

clean:
	rm -f $(BIN).bin $(BIN).hex

re: clean all

.PHONY: all clean re hex flash