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

struct ColorScheme {
    float background[3] = { 0.25f, 0.0f, 0.0f}; 
    float pattern_r[3] = { 9.0f, 1.0f, 3.0f };
    float pattern_g[3] = { 15.0f, 2.0f, 2.0f };
    float pattern_b[3] = { 8.5f, 3.0f, 1.0f };
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
    
    ColorScheme color_scheme;
};
