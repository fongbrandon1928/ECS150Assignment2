# Compiler to use
CC := gcc
# Compilation flags, -c to generate object files, -Wall for all warnings
CFLAGS := -Wall -Werror -Wextra -c
# Archiver to use for creating static libraries
AR := ar
# Flags for the AR command
ARFLAGS := rcs

# Target library
LIB := libuthread.a

# Source directory (adjust this if your source files are in a different directory)
SRC_DIR := .
# Object files directory (adjust this if you want to place object files elsewhere)
OBJ_DIR := .

# Source files (add more .c files to the list as you expand your library)
SOURCES := $(SRC_DIR)/queue.c $(SRC_DIR)/uthread.c $(SRC_DIR)/context.c
# Object files (corresponding to the source files)
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
