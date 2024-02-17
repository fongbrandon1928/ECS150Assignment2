# Compiler to use
CC := gcc
# Compilation flags, -c to generate object files
CFLAGS := -Wall -Werror -Wextra -c
# Archiver for static library
AR := ar
# Flags for the AR command
ARFLAGS := rcs

# Target library
LIB := libuthread.a

# Source directory
SRC_DIR := .
# Object files directory
OBJ_DIR := .

# Source files
SOURCES := $(SRC_DIR)/queue.c $(SRC_DIR)/uthread.c $(SRC_DIR)/context.c $(SRC_DIR)/preempt.c $(SRC_DIR)/sem.c
# Object files
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default target: build the library
all: $(LIB)

# Rule to create the static library from object files
$(LIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

# Pattern rule for object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(LIB)

# Phony targets
.PHONY: all clean
