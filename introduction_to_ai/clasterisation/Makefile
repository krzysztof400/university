# Simplified Makefile to make each .cpp file executable

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -O2

# Default target: build all executables
all: $(patsubst %.cpp, %, $(wildcard *.cpp))

# Rule to build each executable
%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean build files
clean:
	rm -f $(patsubst %.cpp, %, $(wildcard *.cpp))

# Phony targets
.PHONY: all clean
