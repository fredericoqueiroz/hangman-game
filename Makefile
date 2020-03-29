SHELL:=/bin/bash -O globstar

BIN_NAME_1 = servidor
BIN_NAME_2 = cliente

SRC_NAME_1=TCPserver4
SRC_NAME_2=TCPclient4

CC = gcc
CFLAGS = -Wall -Wextra

BIN_DIR = .
SRC_DIR = ./src
OBJ_DIR = ./src/obj

all: setup server client

server: $(SRC_NAME_1).o
	$(CC) $(OBJ_DIR)/$(SRC_NAME_1).o -o $(BIN_NAME_1)

$(SRC_NAME_1).o:
	$(CC) -c $(SRC_DIR)/$(SRC_NAME_1).c -o $(OBJ_DIR)/$(SRC_NAME_1).o $(CFLAGS)

client: $(SRC_NAME_2).o
	$(CC) $(OBJ_DIR)/$(SRC_NAME_2).o -o $(BIN_NAME_2)

$(SRC_NAME_2).o:
	$(CC) -c $(SRC_DIR)/$(SRC_NAME_2).c -o $(OBJ_DIR)/$(SRC_NAME_2).o $(CFLAGS)

setup:
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/$(BIN_NAME_1) $(BIN_DIR)/$(BIN_NAME_2)
	rmdir $(OBJ_DIR)
