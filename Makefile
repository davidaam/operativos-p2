main: misc.o main.o
	gcc -g main.o misc.o -o main

main.o: main.c
	gcc -c main.c

misc.o: misc.c misc.h
	gcc -c misc.c