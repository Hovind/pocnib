# Which compiler to use
CC = gcc

# Compiler flags go here.
CFLAGS = -g -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wstrict-prototypes -Wmissing-prototypes -Wconversion 

# Linker flags go here.
LDFLAGS =

# list of sources
SRC = main.c matrix.c

# program executable file name.
TARGET = kalman

# top-level rule, to compile everything.
all: $(TARGET)

# Define all object files.
OBJ = $(SRC:.c=.o)

# rule to link the program
$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compile: create object files from C source files.
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

# rule for cleaning re-compilable files.
clean:
	rm -f $(TARGET) $(OBJ)

rebuild:	clean all

.PHONY: all rebuild clean
