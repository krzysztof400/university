CXX := g++
CXXFLAGS := -O2 -std=c++17 -pipe -Wall -Wextra
SRC_DIR := src
BIN_DIR := bin
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
BINS := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%,$(SRCS))

.PHONY: all clean dirs run

all: dirs $(BINS)

dirs:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(SRC_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	@rm -rf $(BIN_DIR)
	@echo "Cleaned $(BIN_DIR)"
