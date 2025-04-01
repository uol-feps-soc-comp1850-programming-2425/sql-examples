# Compiler and flags
CC = gcc
CFLAGS = -std=c11
LDFLAGS = -lsqlite3

# Targets
TARGET = example
SRC = example.c
OBJ = $(SRC:.c=.o)

# Default build (optimized)
.PHONY: all
all: CFLAGS += -O2
all: $(TARGET)

# Debug build (with symbols)
.PHONY: debug
debug: CFLAGS += -g -O0
debug: $(TARGET)

# Build the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ)

# Run the program
.PHONY: run
run: $(TARGET)
	./$(TARGET)