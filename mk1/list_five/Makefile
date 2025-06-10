# Universal Makefile to compile each *.cpp file into its own executable

# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17

# Source files and executables
SOURCES := $(wildcard *.cpp)
EXECUTABLES := $(SOURCES:.cpp=)

# Default target: build all executables
all: $(EXECUTABLES)

# Rule to create an executable for each .cpp file
%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean up build files
clean:
	rm -f $(EXECUTABLES)

# Phony targets
.PHONY: all clean
