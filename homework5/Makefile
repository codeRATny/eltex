.PHONY: build run all

OBJ_DIR = build/obj
BIN_DIR = build/bin
CC      = gcc

all: $(OBJ_DIR)/task1.o $(OBJ_DIR)/task2.o $(OBJ_DIR)/write_to_file.o
	$(CC) -Wall -o $(BIN_DIR)/task1 $(OBJ_DIR)/task1.o $(OBJ_DIR)/write_to_file.o
	$(CC) -Wall -o $(BIN_DIR)/task2 $(OBJ_DIR)/task2.o $(OBJ_DIR)/write_to_file.o
	

$(OBJ_DIR)/task1.o: task1.c
	$(CC) -I./write_to_file -Wall -c -o $(OBJ_DIR)/task1.o task1.c

$(OBJ_DIR)/task2.o: task2.c
	$(CC) -I./write_to_file -Wall -c -o $(OBJ_DIR)/task2.o task2.c

$(OBJ_DIR)/write_to_file.o: write_to_file/write_to_file.c
	$(CC) -Wall -c -o $(OBJ_DIR)/write_to_file.o write_to_file/write_to_file.c

run_task1:
	./build/bin/task1

run_task2:
	./build/bin/task2

build_clear:
	rm ./build/bin/* && rm ./build/obj/*