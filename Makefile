# Makefile for pe_calculadora project
# Compiles C code starting from main.c using GCC with warnings enabled

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I$(SRC_DIR)
TARGET = pe_calculadora

# Directories
OUTPUT_DIR = output
SRC_DIR = src

# Source files
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/BigInt/BigInt.c $(SRC_DIR)/LinkedList/LinkedList.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(OUTPUT_DIR) $(OUTPUT_DIR)/$(TARGET)

# Create output directory if it doesn't exist
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# Link the executable
$(OUTPUT_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS)
	rm -rf $(OUTPUT_DIR)

# Run tests
test: $(OUTPUT_DIR) $(OUTPUT_DIR)/test_bigint
	./$(OUTPUT_DIR)/test_bigint

# Compile test program
$(OUTPUT_DIR)/test_bigint: src/test_bigint.c $(SRC_DIR)/BigInt/BigInt.c $(SRC_DIR)/LinkedList/LinkedList.c
	$(CC) $(CFLAGS) -o $@ $^

# Phony targets
.PHONY: all clean test
