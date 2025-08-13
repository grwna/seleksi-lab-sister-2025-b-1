# --- Compilers & Flags ---
CC = g++
NVCC = nvcc

ARCH = -arch=sm_86 # <---------- Sesuaikan ini (nvidia-smi --query-gpu=compute_cap --format=csv,noheader)
CFLAGS = -Wall -Wextra -g -std=c++17 -Isrc/headers -fopenmp
NVCCFLAGS = -g -std=c++17 -Isrc/headers $(ARCH)
LFLAGS_GUI = -lsfml-graphics -lsfml-window -lsfml-system -lGL

# --- Directories ---
SRC = src
LIB = lib
BIN = bin
OBJ = obj

# --- Source & Object Files ---
CLI = $(SRC)/cli.cpp
GUI = $(SRC)/gui/JuliaWindow.cpp $(SRC)/gui/MandelbrotWindow.cpp $(SRC)/gui/ControlWindow.cpp $(SRC)/gui.cpp        
BACKEND = $(SRC)/generator/image_writer.cpp $(SRC)/generator/serial.cpp $(SRC)/generator/cpu.cpp
CU  = $(SRC)/generator/gpu.cu
IMGUI = $(wildcard $(LIB)/*.cpp)

CLI_OBJECTS = $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(CLI))
GUI_OBJECTS = $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(GUI))
BACKEND_OBJECTS = $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(BACKEND))
CU_OBJECTS  = $(patsubst $(SRC)/%.cu,$(OBJ)/%.o,$(CU))
IMGUI_OBJECTS = $(patsubst $(LIB)/%.cpp,$(OBJ)/%.o,$(IMGUI))

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

$(OBJ)/%.o: $(LIB)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling ImGui: $<"
	$(CC) $(CFLAGS) -c $< -o $@


# Kompilasi CUDA (.cu -> .o)
$(OBJ)/%.o: $(SRC)/%.cu
	@mkdir -p $(dir $@)
	@echo "Compiling CUDA: $<"
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

# Linking CLI 
$(CLI_BIN): $(CLI_OBJECTS) $(CU_OBJECTS) $(BACKEND_OBJECTS)
	@mkdir -p $(BIN)
	@echo "Linking $(CLI_BIN)..."
	$(NVCC) $^ -o $@ -Xcompiler "$(CFLAGS)"

# Linking GUI
$(GUI_BIN): $(GUI_OBJECTS) $(BACKEND_OBJECTS) $(CU_OBJECTS) $(IMGUI_OBJECTS) 
	@mkdir -p $(BIN)
	@echo "Linking $(GUI_BIN)..."
	$(NVCC) $^ -o $@ -Xcompiler "$(CFLAGS)" $(LFLAGS_GUI)

# --- Utility Targets ---
run_cli: cli
	@./$(CLI_BIN)

run_gui: gui
	@./$(GUI_BIN)

run_benchmark: benchmark
	@$(BIN)/benchmark

clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ) $(BIN)