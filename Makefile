CC = gcc
CFLAGS = -Wall -g -O0
SRC = $(wildcard src/*.c)
OUT = "./bin/program"


all: 
	make build 
	make run

build:
	mkdir -p "./bin/"
	rm -f $(OUT) || true 
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

run:
	$(OUT) 

clear:
	rm -f $(OUT) || true 
