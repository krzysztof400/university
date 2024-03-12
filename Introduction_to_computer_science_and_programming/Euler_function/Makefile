$(CC) = gcc

final: main.o funeul.o
	$(CC) main.o funeul.o -o final

main.o: main.c header.h 
	$(CC) -c main.c

funeul.o: funeul.c header.h
	$(CC) -c funeul.c

clean:
	rm *.o final