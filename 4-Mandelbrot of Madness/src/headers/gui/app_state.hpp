#pragma once

enum class Mode {
    NONE,
    SERIAL,
    CPU,
    GPU
};

// Default Params
struct RenderParameters {
    int width = 800;
    int height = 600;
    int max_iterations = 20;
    Mode mode = Mode::SERIAL;
};

struct SharedState {
    RenderParameters params;
    bool is_running = true;
    
    // tells the renderer when to render
    bool needs_update = true; 

};
