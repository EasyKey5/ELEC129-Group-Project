CC = gcc

all: rentals

rentals: customers.c films.c main.c
	$(CC) "./customers.c" "./main.c" "./films.c" -o "./program" -Wall -g && "./program" 
