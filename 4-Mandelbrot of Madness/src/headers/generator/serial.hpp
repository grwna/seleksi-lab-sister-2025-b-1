#pragma once
#include "datatypes.hpp"
#include "image_writer.hpp"
#include <vector>

int cardioidCheck(float real, float imag, int max_iters);

int calculateMandelbrot(const Complex &c, int max_iters);

Colors juliaSerial(int width, int height, int max_iterations, const Complex& c);
Colors mandelbrotSerial(int width, int height, int max_iterations, const Bounds& bounds);
