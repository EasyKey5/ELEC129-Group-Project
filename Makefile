CC = gcc

all: rentals

rentals: customers.c films.c main.c
	rm -f "./bin/program" || true 
	mkdir -p "./bin/"
	$(CC) "./customers.c" "./main.c" "./films.c" -o "./bin/program" -Wall -g
	"./bin/program" 
	echo "./main.c"
