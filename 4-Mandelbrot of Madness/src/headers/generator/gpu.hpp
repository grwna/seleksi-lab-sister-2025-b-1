#pragma once

#include "../datatypes.hpp"
#include "../gui/SharedState.hpp"
#include <vector>



Colors mandelbrotGPU(int width, int height, int max_iterations, const Bounds& bounds, const ColorScheme& scheme);
Colors juliaGPU(int width, int height, int max_iterations, const Complex& c, const ColorScheme& scheme);