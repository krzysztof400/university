SOURCES = $(wildcard *.c)
BINARY = $(patsubst %.c, %, $(SOURCES))

CC = gcc
FLAGS = -W -pedantic -std=c2x -O3
LIBS = 

.PHONY: all clean debug

all: $(BINARY)

%: %.c
	$(CC) $(FLAGS) $< -o $@ $(LIBS)
	strip $@

debug: FLAGS = -W -pedantic -std=c2x -g
debug: $(BINARY)

clean:
	rm -f $(BINARY)