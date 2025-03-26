CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17

# Source files and executable names
SOURCES = insertion_sort.cpp quick_sort.cpp random_generator.cpp ascending_generator.cpp descending_generator.cpp
EXECUTABLES = $(SOURCES:.cpp=)

# Default rule: build all executables
all: $(EXECUTABLES)

# Compile each program from its corresponding source file
$(EXECUTABLES): %: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean up compiled files
clean:
	rm -f $(EXECUTABLES)

# .PHONY to avoid conflicts with file names
.PHONY: all clean