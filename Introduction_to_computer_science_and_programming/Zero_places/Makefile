$(CC) = gcc

final: main.o f.o rozwiazania.o
	$(CC) main.o f.o rozwiazania.o -o final -lm

main.o: main.c header.h 
	$(CC) -c main.c

f.o: f.c header.h
	$(CC) -c f.c -o f.o -lm

rozwiazania.o: rozwiazania.c header.h 
	$(CC) -c rozwiazania.c -o rozwiazania.o -lm

clean:
	rm *-o final

