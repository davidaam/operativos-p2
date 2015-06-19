main: misc.o main.o
	gcc -Wall -g main.o misc.o -o main

main.o: main.c
	gcc -c -g main.c

misc.o: misc.c misc.h
	gcc -c -g misc.c

clean:
	rm *.o
	rm main