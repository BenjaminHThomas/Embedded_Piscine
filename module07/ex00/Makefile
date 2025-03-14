# Board specs
MCU		= atmega328p
F_CPU	= 16000000
BAUD	= 115200

# Compile args
SRCDIR	= src
OBJDIR	= obj
CFILES	= ex00.c \
			uart.c \
			eeprom.c
CC		= avr-gcc
OBJCOPY	= avr-objcopy
INSTALL	= avrdude
FLAGS	= -Wall -Werror -Wextra -O3
FLAGS	+= -DF_CPU=$(F_CPU) -mmcu=$(MCU)
FLAGS	+= -Wl,-u,vfprintf -lprintf_flt -lm
INCS	= -I ./include
PORT	= /dev/ttyUSB0

# Compile instructions
vpath %.c $(SRCDIR)

OBJS = $(addprefix $(OBJDIR)/, $(CFILES:.c=.o))
BIN  = $(OBJDIR)/firmware.bin
HEX  = $(OBJDIR)/firmware.hex

all: flash

# Compile object files
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(FLAGS) $(INCS) -c $< -o $@

# Link object files
$(BIN): $(OBJS)
	$(CC) $(FLAGS) $^ -o $@

# Convert binary to hex
$(HEX): $(BIN)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

flash: $(HEX)
	$(INSTALL) -F -V -c arduino -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$(HEX):i

clean:
	rm -rf $(OBJDIR)

tidyin: all clean

re: clean all

.PHONY: all clean re flash tidyin
