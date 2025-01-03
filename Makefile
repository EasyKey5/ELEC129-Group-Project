CC = gcc
FLAGS = -Wall -g
SRC = $(wildcard src/*.c)
OUT = "./bin/program"


all: 
	make build 
	make run

build:
	mkdir -p "./bin/"
	rm -f $(OUT) || true 
	$(CC) $(SRC) -o $(OUT) $(CFLAGS)

run:
	$(OUT) 

clear:
	rm -f $(OUT) || true 
