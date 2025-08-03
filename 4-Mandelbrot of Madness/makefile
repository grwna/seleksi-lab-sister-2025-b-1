CC = g++
CFLAGS = -Wall -Wextra -g -std=c++17 -Isrc/headers

# Directories
SRC = src
BIN = bin
OBJ = obj

CLI_BIN = $(BIN)/mandelbrot_cli
GUI_BIN = $(BIN)/mandelbrot_gui

CLI_SOURCES = $(SRC)/cli.cpp $(SRC)/serial.cpp $(SRC)/cpu.cpp $(SRC)/image_writer.cpp
CLI_OBJECTS = $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(CLI_SOURCES))

GUI_SOURCES = $(SRC)/gui/components.cpp $(SRC)/gui/control_window.cpp $(SRC)/gui/render_window.cpp $(SRC)/gui.cpp \
			  $(SRC)/gpu.cpp $(SRC)/image_writer.cpp $(SRC)/serial.cpp $(SRC)/cpu.cpp
GUI_OBJECTS = $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(GUI_SOURCES))

cli: $(CLI_BIN)
gui: $(GUI_BIN)
all: cli gui

$(OBJ)/%.o: $(SRC)/%.cpp
	@mkdir -p $(OBJ)/gui
	@mkdir -p $(OBJ)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(CLI_BIN): $(CLI_OBJECTS)
	@mkdir -p $(BIN)
	@echo "Linking $(CLI_BIN)..."
	$(CC) $(CLI_OBJECTS) -o $@

$(GUI_BIN): $(GUI_OBJECTS)
	@mkdir -p $(BIN)
	@echo "Linking $(GUI_BIN)..."
	$(CC) $(GUI_OBJECTS) -o $@ -lsfml-graphics -lsfml-window -lsfml-system # Example for SFML

.PHONY: all clean run_cli run_gui

run_cli: $(CLI_BIN)
	@echo "Running $(CLI_BIN)..."
	@$(CLI_BIN)

run_gui: $(GUI_BIN)
	@echo "Running $(GUI_BIN)..."
	@$(GUI_BIN)

clean:
	@echo "Cleaning up..."
	@rm -f $(CLI_BIN) $(GUI_BIN)
	@rm -rf $(OBJ)
	@rmdir --ignore-fail-on-non-empty $(BIN)