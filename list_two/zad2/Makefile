CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17

# Shared utility source
UTIL = utils.cpp

# Source files and executable names
SOURCES = random_generator.cpp merge_sort.cpp quick_sort.cpp insertion_sort.cpp DP_quick_sort.cpp descending_generator.cpp ascending_generator.cpp tim_sort.cpp
EXECUTABLES = $(SOURCES:.cpp=)

# Default rule: build all executables
all: $(EXECUTABLES)

# Compile each program along with utils.cpp
$(EXECUTABLES): %: %.cpp $(UTIL)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

# Clean up compiled files
clean:
	rm -f $(EXECUTABLES)

# .PHONY to avoid conflicts with file names
.PHONY: all clean
