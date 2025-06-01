# Makefile for building all C programs in the current directory

CC = gcc
CFLAGS = -Wall -Wextra -O2
SOURCES = $(wildcard *.c)
EXECUTABLES = $(SOURCES:.c=)

all: $(EXECUTABLES)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLES) *.o

.PHONY: all clean