SIL ?=

CROSS = avr

TARGET = blinky

CC = $(CROSS)-gcc
OBJCOPY = $(CROSS)-objcopy
OBJDUMP = $(CROSS)-objdump
SIZE = $(CROSS)-size

AVRDUDE = avrdude

CFLAGS := -g -Os -std=gnu11
CFLAGS := -ffunction-sections -fdata-sections $(CFLAGS)
CFLAGS := -flto -fno-fat-lto-objects $(CFLAGS)
CFLAGS := -mmcu=atmega328p -DF_CPU=16000000L $(CFLAGS)
CFLAGS := -Wall -Werror  $(CFLAGS)

LDFLAGS := -Wl,-Map,$(TARGET).map

SRCS = main.c uart.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET).hex $(TARGET).lst

$(TARGET).hex: $(TARGET).elf
	@echo "\n\033[1;34mConverting to Intel HEX format...\033[0m"
	$(SIL)$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo "\n\033[1;36mSize summary:\033[0m"
	$(SIL)$(SIZE) $<

$(TARGET).lst: $(TARGET).elf
	@echo "\n\033[1;36mGenerating listing...\033[0m"
	$(SIL)$(OBJDUMP) -h -S $< > $@

$(TARGET).elf: $(OBJS)
	@echo "\n\033[1;34mLinking $@...\033[0m"
	$(SIL)$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	@echo "\033[1;32mBuilding $<...\033[0m"
	$(SIL)$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	@echo "\033[1;34mCleaning...\033[0m"
	$(SIL)rm -f $(TARGET).hex $(TARGET).elf
	$(SIL)rm -f $(TARGET).lst $(TARGET).map
	$(SIL)rm -f *.o

flash:
