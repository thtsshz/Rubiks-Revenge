# Makefile for compiling rubik.cpp

# Compiler
CC = g++

# Compiler flags
CFLAGS = -std=c++11 -Wall

# Source file
SRC = rubik.cpp

# Executable name
EXEC = rubik

# Default target
all: $(EXEC)

# Target for compiling the executable
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Target for cleaning generated files
clean:
	rm -f $(EXEC)
