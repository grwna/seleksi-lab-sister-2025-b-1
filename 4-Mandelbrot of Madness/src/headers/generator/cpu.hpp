# include <omp.h>
#include "serial.hpp"

Colors mandelbrotCPU(int width, int height, int max_iterations, const Bounds& bounds);
Colors juliaCPU(int width, int height, int max_iterations, const Complex& c);