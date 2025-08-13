#pragma once

#include "../datatypes.hpp"
#include <vector>



Colors mandelbrotGPU(int width, int height, int max_iterations, const Bounds& bounds);
Colors juliaGPU(int width, int height, int max_iterations, const Complex& c);