# --- Compilers & Flags ---
CC = g++
NVCC = nvcc

ARCH = -arch=sm_86 # <---------- Sesuaikan ini (nvidia-smi --query-gpu=compute_cap --format=csv,noheader)
CFLAGS = -Wall -Wextra -g -std=c++17 -Isrc/headers -fopenmp
NVCCFLAGS = -g -std=c++17 -Isrc/headers $(ARCH)
LFLAGS_GUI = -lsfml-graphics -lsfml-window -lsfml-system

# --- Directories ---
SRC = src
BIN = bin
OBJ = obj

# --- Source & Object Files ---
CLI_CPP_SOURCES = $(SRC)/cli.cpp $(SRC)/generator/serial.cpp $(SRC)/generator/cpu.cpp $(SRC)/image_writer.cpp
CLI_CU_SOURCES  = $(SRC)/generator/gpu.cu
GUI_CPP_SOURCES = $(SRC)/gui/components.cpp $(SRC)/gui/control_window.cpp $(SRC)/gui/render_window.cpp $(SRC)/gui.cpp \
                  $(SRC)/image_writer.cpp $(SRC)/generator/serial.cpp $(SRC)/generator/cpu.cpp
GUI_CU_SOURCES  = $(SRC)/generator/gpu.cu

CLI_CPP_OBJECTS = $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(CLI_CPP_SOURCES))
CLI_CU_OBJECTS  = $(patsubst $(SRC)/%.cu,$(OBJ)/%.o,$(CLI_CU_SOURCES))
GUI_CPP_OBJECTS = $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(GUI_CPP_SOURCES))
GUI_CU_OBJECTS  = $(patsubst $(SRC)/%.cu,$(OBJ)/%.o,$(GUI_CU_SOURCES))

# --- Binaries ---
CLI_BIN = $(BIN)/mandelbrot_cli
GUI_BIN = $(BIN)/mandelbrot_gui

.PHONY: all cli gui clean run_cli run_gui

all: cli gui
cli: $(CLI_BIN)
gui: $(GUI_BIN)
benchmark: 
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $(SRC)/benchmark/benchmark.cpp -o $(BIN)/benchmark

# --- Rules ---

# Kompilasi C++ (.cpp -> .o)
$(OBJ)/%.o: $(SRC)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling C++: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Kompilasi CUDA (.cu -> .o)
$(OBJ)/%.o: $(SRC)/%.cu
	@mkdir -p $(dir $@)
	@echo "Compiling CUDA: $<"
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

# Linking CLI 
$(CLI_BIN): $(CLI_CPP_OBJECTS) $(CLI_CU_OBJECTS)
	@mkdir -p $(BIN)
	@echo "Linking $(CLI_BIN)..."
	$(NVCC) $^ -o $@ -Xcompiler "$(CFLAGS)"

# Linking GUI
$(GUI_BIN): $(GUI_CPP_OBJECTS) $(GUI_CU_OBJECTS)
	@mkdir -p $(BIN)
	@echo "Linking $(GUI_BIN)..."
	$(NVCC) $^ -o $@ $(LFLAGS_GUI) -Xcompiler "$(CFLAGS)"

# --- Utility Targets ---
run_cli: cli
	@./$(CLI_BIN)

run_gui: gui
	@./$(GUI_BIN)

run_benchmark: benchmark
	@bin/benchmark

clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ) $(BIN)