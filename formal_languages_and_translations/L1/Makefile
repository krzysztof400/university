CC := g++
CFLAGS := -O2 -std=c++17 -Wall -Wextra
FA_SRC := fa.cpp
KMP_SRC := kmp.cpp
FA_BIN := FA
KMP_BIN := KMP

PATTERN := "limits of my world"
TRACTATUS_FILE := tractatus.txt

.PHONY: all fa kmp clean test run_fa run_kmp

all: $(FA_BIN) $(KMP_BIN)

$(FA_BIN): $(FA_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(KMP_BIN): $(KMP_SRC)
	$(CC) $(CFLAGS) -o $@ $^

fa: $(FA_BIN)
kmp: $(KMP_BIN)

test: all $(TRACTATUS_FILE)
	@echo "Running FA on $(TRACTATUS_FILE) ..."
	time ./$(FA_BIN) $(PATTERN) $(TRACTATUS_FILE) > fa_matches.txt || true
	@echo "FA results written to fa_matches.txt"
	@echo "Running KMP on $(TRACTATUS_FILE) ..."
	time ./$(KMP_BIN) $(PATTERN) $(TRACTATUS_FILE) > kmp_matches.txt || true
	@echo "KMP results written to kmp_matches.txt"
	@echo "Done."

clean:
	@rm -f $(FA_BIN) $(KMP_BIN) fa_matches.txt kmp_matches.txt $(TRACTATUS_FILE)
	@echo "Cleaned."
