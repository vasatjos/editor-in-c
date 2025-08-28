CC = gcc

all: kilo

run : kilo
	./kilo && reset

kilo: main.c
	$(CC) main.c -o kilo -Wall -Wextra -pedantic -std=c99
