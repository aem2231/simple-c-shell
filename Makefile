# Makefile
# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lreadline
BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = out

# Default target
all: $(BIN_DIR)/program

# Compile source files into object files
$(BIN_DIR)/program: $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/shell $(OBJ_DIR)/main.o $(LDFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o

# Create out dir
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/out
