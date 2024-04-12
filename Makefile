#----------------------------------------------------------
# Compiler
#----------------------------------------------------------
CC=avr-gcc
CFLAGS=-Wall -Os
OBJCOPY=avr-objcopy
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
SOURCES=main.c
OBJECTS=$(SOURCES:%.c=$(OBJ_DIR)/%.o)
EXECUTABLE=$(BIN_DIR)/main.elf
BINARY=$(BIN_DIR)/main.hex

#----------------------------------------------------------
# Board
#----------------------------------------------------------
MCU=atmega328p
CPU=16000000

#----------------------------------------------------------
# Programmer
#----------------------------------------------------------
PORT=/dev/ttyUSB0
BAUD=57600
PROGRAMMER=arduino
PARTNO=m328p

all: dirs $(BINARY)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -DF_CPU=$(CPU) -mmcu=$(MCU) -c $^ -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) -DF_CPU=$(CPU) -mmcu=$(MCU) -o $@ $^

$(BINARY): $(EXECUTABLE)
	$(OBJCOPY) -O ihex $< $@

flash: all
	avrdude -v -c $(PROGRAMMER) -p $(PARTNO) -P $(PORT) -b $(BAUD) -D -U flash:w:$(BINARY):i

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

.PHONY: all clean dirs flash

