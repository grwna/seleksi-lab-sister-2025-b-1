#pragma once
#include "datatypes.hpp"
#include <vector>

int calculateMandelbrot(const Complex &c, int max_iters);

Colors mandelbrotSerial(int width, int height, int max_iterations);