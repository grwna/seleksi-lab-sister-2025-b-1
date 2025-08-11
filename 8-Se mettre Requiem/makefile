CC = gcc
CFLAGS = -Wall -Wextra -g -Isrc/headers -O2

SRC = src
BIN = bin
OBJ = obj
MAC = macro_index
SUB = subscript_index

MAC_SOURCES = $(SRC)/$(MAC)/main.c $(SRC)/$(MAC)/ntt.c $(SRC)/$(MAC)/operations.c $(SRC)/$(MAC)/utils.c $(SRC)/$(MAC)/symbols.c
MAC_OBJ = $(patsubst $(SRC)/$(MAC)/%.c,$(OBJ)/mac_%.o,$(MAC_SOURCES))

SUB_SOURCES = $(SRC)/$(SUB)/main.c $(SRC)/$(SUB)/ntt.c $(SRC)/$(SUB)/operations.c $(SRC)/$(SUB)/utils.c $(SRC)/$(SUB)/symbols.c
SUB_OBJ = $(patsubst $(SRC)/$(SUB)/%.c,$(OBJ)/sub_%.o,$(SUB_SOURCES))

BINARIES = $(BIN)/macro_index $(BIN)/subscript_index

.PHONY: all
all: $(BINARIES)

# Compile macro_index sources
$(OBJ)/mac_%.o: $(SRC)/$(MAC)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling macro_index: $< -> $@"
	$(CC) $(CFLAGS) -I$(SRC)/$(MAC)/headers -c $< -o $@

# Compile subscript_index sources
$(OBJ)/sub_%.o: $(SRC)/$(SUB)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling subscript_index: $< -> $@"
	$(CC) $(CFLAGS) -I$(SRC)/$(SUB)/headers -c $< -o $@

# Link macro_index binary
$(BIN)/macro_index: $(MAC_OBJ)
	@mkdir -p $(BIN)
	@echo "Linking macro_index..."
	$(CC) $^ -o $@

# Link subscript_index binary
$(BIN)/subscript_index: $(SUB_OBJ)
	@mkdir -p $(BIN)
	@echo "Linking subscript_index..."
	$(CC) $^ -o $@

.PHONY: run_macro run_sub

run: $(BIN)/macro_index
	@$(BIN)/macro_index

run_sub: $(BIN)/subscript_index
	@$(BIN)/subscript_index

.PHONY: clean
clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ) $(BIN)