#pragma once
#include "datatypes.hpp"
#include "../image_writer.hpp"
#include <vector>

int cardioidCheck(double real, double imag, int max_iters);

int calculateMandelbrot(const Complex &c, int max_iters);

Colors mandelbrotSerial(int width, int height, int max_iterations);