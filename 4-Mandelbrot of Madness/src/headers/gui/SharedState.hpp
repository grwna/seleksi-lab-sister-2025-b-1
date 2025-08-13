#pragma once

#include "../datatypes.hpp"

enum class FractalType {
    MANDELBROT,
    JULIA
};

enum class Mode {
    SERIAL,
    CPU,
    GPU
};
    
struct RenderParameters {
    int width;
    int height;
    int max_iterations;
    double zoom;
};

struct SharedState {
    RenderParameters mandelbrot_params = {700, 600, 50, 1};
    RenderParameters julia_params = {400, 400, 50, 1};
    
    bool is_running = true;
    bool mandelbrot_needs_update = true;
    bool julia_needs_update = true;   
    
    Mode mode = Mode::SERIAL;

    bool show_julia_window = false;
    Complex julia_c = { -0.8, 0.156 };

};
