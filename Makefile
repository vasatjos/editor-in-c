CC = gcc

kilo: main.c
	$(CC) main.c -o kilo -Wall -Wextra -pedantic -std=c99
