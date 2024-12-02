# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRCS = main.c Scanner.c Parser.c

# Header files
HEADERS = Scanner.h Parser.h Token.h

# Object files
OBJS = main.o Scanner.o Parser.o

# Executable name
EXEC = main

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

# Clean up build files
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean