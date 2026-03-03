# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Source files and dependencies
SRCS = main.c dpll.c
HDRS = dpll.h

# Output binary
TARGET = main

# Default target
all: build

# Explicit build target with parameters
build: $(TARGET)

# Build target
$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Run with parameters
run: build
	./$(TARGET) $(INPUT) $(OUTPUT)

# Force rebuild (ignores file timestamps)
rebuild: clean build

# Clean up build artifacts
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all build clean run rebuild
