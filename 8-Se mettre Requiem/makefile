# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -g -Isrc/headers

# Directories
SRC = src/
BIN = bin

# Define the names of the executables to be built
EXECUTABLES = $(BIN)/se_mettre

all: $(EXECUTABLES)

$(BIN)/se_mettre: $(SRC)/se_mettre.c
	@mkdir -p $(BIN)
	@echo "Compiling se_mettre.c into $(BIN)/se_mettre"
	$(CC) $(CFLAGS) $< -o $@

run:
	@$(BIN)/se_mettre

.PHONY: all clean

clean:
	@echo "Cleaning up..."
	@rm -f $(EXECUTABLES)
	@rmdir --ignore-fail-on-non-empty $(BIN)

