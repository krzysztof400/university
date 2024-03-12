$(CC) = gcc

all: final

final:  main.o distance.o west.o east.o south.o north.o newagent.o
	$(CC) main.o distance.o west.o east.o south.o north.o newagent.o -o final -lm

main.o: main.c agents.h
	$(CC) -c main.c -o main.o

distance.o: distance.c agents.h
	$(CC) -c distance.c -o distance.o

west.o: west.c agents.h
	$(CC) -c west.c -o west.o

east.o: east.c agents.h
	$(CC) -c east.c -o east.o

south.o: south.c agents.h
	$(CC) -c south.c -o south.o

north.o: north.c agents.h 
	$(CC) -c north.c -o north.o

newagent.o: newagent.c agents.h
	$(CC) -c newagent.c -o newagent.o -lm

clean:
	rm *-o final