CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17

# Shared utility source
UTIL = utils.cpp

# Source files and executable names
SOURCES = generator_random.cpp randomized_select.cpp select.cpp generator_ascending.cpp generator_descending.cpp select_k_divisions.cpp
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
