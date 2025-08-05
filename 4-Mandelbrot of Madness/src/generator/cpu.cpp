#include "../headers/generator/cpu.hpp"

Colors mandelbrotCPU(int width, int height, int max_iters){
    Colors pixels(width * height);

    # pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            double real = -2.0 + (static_cast<double>(x) / (width - 1)) * 3.0;
            double imag = -1.0 + (static_cast<double>(y) / (height - 1)) * 2.0;

            int iterations = cardioidCheck(real, imag, max_iters);
            pixels[y * width + x] = getColor(iterations, max_iters);
        }
    }

    return pixels;  
}
