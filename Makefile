UNAME_S:=$(shell uname -s)

#----------------------------------------------------------
# Compiler
#----------------------------------------------------------
CC=avr-gcc
CFLAGS=-Wall -Os
OBJCOPY=avr-objcopy
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
SOURCES=main.c uart.c circular_buffer.c
OBJECTS=$(SOURCES:%.c=$(OBJ_DIR)/%.o)
EXECUTABLE=$(BIN_DIR)/main.elf
BINARY=$(BIN_DIR)/main.hex

TEST_SRC_DIR=tests
TEST_OBJ_DIR=test_obj
TEST_BIN_DIR=test_bin
TEST_SOURCES=main.c test_circular_buffer.c
TEST_OBJECTS=$(TEST_SOURCES:%.c=$(TEST_OBJ_DIR)/%.o)
TEST_SRC_OBJECTS=$(SOURCES:%.c=$(TEST_OBJ_DIR)/%.o)
TEST_BINARY=$(TEST_BIN_DIR)/tests


#----------------------------------------------------------
# Board
#----------------------------------------------------------
MCU=atmega328p
CPU=16000000

#----------------------------------------------------------
# Programmer
#----------------------------------------------------------
ifeq ($(UNAME_S),Linux)
	PORT=/dev/ttyUSB0
else ifeq ($(UNAME_S),Darwin)
	PORT=/dev/tty.usbserial-2110
else
	echo "Error: unsuported OS"
	exit 1;
endif
BAUD=57600
PROGRAMMER=arduino
PARTNO=m328p

all: dirs $(BINARY)

# Use this rule if a source file has a corresponding header file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) -DF_CPU=$(CPU) -mmcu=$(MCU) -c $< -o $@

# Use this rule if a source file doesn't have a corresponding header file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -DF_CPU=$(CPU) -mmcu=$(MCU) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) -DF_CPU=$(CPU) -mmcu=$(MCU) -o $@ $^

$(BINARY): $(EXECUTABLE)
	$(OBJCOPY) -O ihex $< $@

flash: all
	avrdude -v -c $(PROGRAMMER) -p $(PARTNO) -P $(PORT) -b $(BAUD) -D -U flash:w:$(BINARY):i

serial:
	picocom -b 9600 $(PORT)

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(TEST_OBJ_DIR)
	mkdir -p $(TEST_BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(TEST_OBJ_DIR)
	rm -rf $(TEST_BIN_DIR)

setup-mac:
	brew tap osx-cross/avr
	brew install avr-gcc avrdude make picocom

.PHONY: all clean dirs flash

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.c $(TEST_SRC_DIR)/%.h
	gcc $(CFLAGS) -I../$(SRC_DIR) -c $< -o $@

# Use this rule if a source file doesn't have a corresponding header file
$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.c
	gcc $(CFLAGS) -I../$(SRC_DIR) -c $< -o $@

$(TEST_BINARY): $(TEST_OBJECTS)
	gcc -o $@ $^

test: dirs $(TEST_BINARY)
	./$(TEST_BINARY)