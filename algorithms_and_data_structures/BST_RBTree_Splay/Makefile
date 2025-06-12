# Makefile for building all C++ programs in the current directory

CXX = g++
CXXFLAGS = -Wall -Wextra -O2
SOURCES = $(wildcard *.cpp)
EXECUTABLES = $(SOURCES:.cpp=)

all: $(EXECUTABLES)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLES) *.o

.PHONY: all clean