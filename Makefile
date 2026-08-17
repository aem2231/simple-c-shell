# Makefile
# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -I./include
LDFLAGS = -lreadline
BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = out
TARGET = shell
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/shell_builtins.o $(OBJ_DIR)/terminal.o

# Default target
all: $(BIN_DIR)/$(TARGET)

# Portable build (works on any Linux with musl)
portable: clean $(BIN_DIR)/$(TARGET)

# Create bin dir
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Create out dir
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Link object files into binary
$(BIN_DIR)/$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJS) $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/shell_builtins.o: $(SRC_DIR)/shell_builtins.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/shell_builtins.c -o $(OBJ_DIR)/shell_builtins.o

$(OBJ_DIR)/terminal.o: $(SRC_DIR)/terminal.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/terminal.c -o $(OBJ_DIR)/terminal.o

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
