$(CC) = gcc

final: main.o palindrom.o
	$(CC) main.o palindrom.o -o final

main.0: main.c
	$(CC) -c main.c

palindrom.o: palindrom.c
	$(CC) -c palindrom.c

clean: 
	rm *.o -final